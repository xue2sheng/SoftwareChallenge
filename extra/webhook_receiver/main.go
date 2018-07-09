package main

import (
	"flag"
	"log"
	"os"
	"os/exec"

	"gopkg.in/go-playground/webhooks.v3"
	"gopkg.in/go-playground/webhooks.v3/github"
)

const (
	pathDefault = "/webhooks"
	portDefault = "3016"
	triggerDefault = "CMakeLists.txt"
	buildDefault = "/home/user/Code/SoftwareChallenge/build"
	sourceDefault = "/home/user/Code/SoftwareChallenge"
)

// global due to laziness
var trigger *string
var build *string
var source *string

func main() {

	// commandline arguments
	secret := flag.String("secret", "", "Secret to connect to the repository. (Required)")
	path := flag.String("path", pathDefault, "Webhooks receiver path to listen to.")
	port := flag.String("port", portDefault, "Webhooks receiver port to listen to.")
	trigger = flag.String("trigger", triggerDefault, "Webhooks receiver trigger file to provoke some action.")
	build = flag.String("build", buildDefault, "Project build directory where doc/Doxyfile is located.")
	source = flag.String("source", sourceDefault, "Project source directory where doc/Doxyfile.in is located.")
	debug := flag.Bool("debug", false, "Launch in debug mode.")
	flag.Parse()

	// debugging
	if *debug {
		RegenerateDoc(*source, *build)
		os.Exit(0)
	}

	// secret is required
	if len(*secret) == 0 {
		log.Fatal("Missing required 'secret' command line argument")
	}

	hook := github.New(&github.Config{Secret: *secret})
	hook.RegisterEvents(HandleMultiple, github.ReleaseEvent, github.PushEvent) // Add as many as you want

	err := webhooks.Run(hook, ":"+ *port, *path)
	if err != nil {
		log.Fatal(err)
	}
}

// HandleMultiple handles multiple GitHub events
func HandleMultiple(payload interface{}, header webhooks.Header) {

	log.Println("Handling Payload with trigger file " + *trigger + " ...")

	switch payload.(type) {

	case github.PushPayload:
		push := payload.(github.PushPayload)
		for _, item := range push.HeadCommit.Modified {
			if (*trigger == item) {
				RegenerateDoc(*source,*build)
				break;
			}
		} //for
	}
}

// RegenerateDoc get the latest source code and regenerate its documentation
func  RegenerateDoc(source_dir string, build_dir string) {

	// git pull if trigger file was modified. 
	// git -C <<source_dir>> pull
	cmd := exec.Command("git", "-C", source_dir, "pull")
	log.Printf("Git pull...")
	err := cmd.Run()
	if err != nil {
		log.Printf("Git pull finished with error: %v", err)
	} else {
		log.Printf("Git pull finished successfully")
	}

	// update Doxygen documentation

	// cmake -H<<source dir>> -B<<build_dir>>
	cmd = exec.Command("cmake","-H"+*source,"-B"+*build,"-DEXTRA:STRING=yes")
	log.Printf("Updating cmake with extra content..")
	err = cmd.Run()
	if err != nil {
		log.Printf("cmake finished with error: %v", err)
	} else {
		log.Printf("cmake finished successfully")
	}

	// cmake --build <<build_dir>> --target regenerate_doc 
	cmd = exec.Command("cmake","--build",*build,"--target","regenerate_doc")
	log.Printf("Updating doxygen documentation..")
	err = cmd.Run()
	if err != nil {
		log.Printf("doxygen finished with error: %v", err)
	} else {
		log.Printf("doxygen finished successfully")
	}

}
