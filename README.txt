3 FILES 
- main.cpp
	renderScene()
	main()

- functions.h
	processInput()
	mouseCallback()
	frameBufferSizeCallback()
	initialize()
	loadOBJ()

- Shader.h
	class Shader {
		Shader()
		use()
		update()
		setInt()
		setFloat()
		setVec3()
		setMat()
	}

- Camera.h
	class Camera {
		Camera()
		ProcessKeyboard()
		ProcessMouseMovement()
		updateCameraVectors()
	}

----------------------------------------------------------------------------------------

CONTROLS 
- left 	move the camera to the left
- right move the camera to the right
- up 	move the camera into the scene
- down	move the camera out of the scene 

- A	move the light to the left
- D	move the light to the right
- W	move the light up
- S	move the light down

- P	increase the bias 
- O 	decrease the bias

- esc	close the window