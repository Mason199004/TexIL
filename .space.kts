/**
* JetBrains Space Automation
* This Kotlin-script file lets you automate build activities
* For more info, see https://www.jetbrains.com/help/space/automation.html
*/

job("build") {
    parallel {
    	container("ubuntu")
        {
        	  sequential 
          	  {
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 """
                }
                shellScript
                {
                	content = """
                 	cmake --build ./build
                 """
                }
          	  }
        }
        container("alpine")
        {
        	  sequential 
          	  {
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 """
                }
                shellScript
                {
                	content = """
                 	cmake --build ./build
                 """
                }
          	  }
        }
        container("manjaro")
        {
        	  sequential 
          	  {
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 """
                }
                shellScript
                {
                	content = """
                 	cmake --build ./build
                 """
                }
          	  }
        }
        container("macos")
        {
        	  sequential 
          	  {
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 """
                }
                shellScript
                {
                	content = """
                 	cmake --build ./build
                 """
                }
          	  }
        }
        container("windows")
        {
        	  sequential 
          	  {
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 """
                }
                shellScript
                {
                	content = """
                 	cmake --build ./build
                 """
                }
          	  }
        }
    }
}
