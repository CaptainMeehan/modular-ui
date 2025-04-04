For some reason UI was one of the things people didn't seem all too eager to take so I wanted to create a modular UI that is easy to implement in projects. 
The UI itself is easy to use for a programmer but I also wanted it to be possible to export UI from a UI editor to a JSON file using nlohmann and then be able
to load those json files directly into my modular UI creating all the nescessary classes, functions and variables to keep the structure of the UI from the editor.

Note that this project is ongoing and some parts of the code might not make sense as it is right now implemented in another system. 
The code is centered around a UI type base class and a UI manager with a state stack.

Next step in this code is cleanup and a more general approach to load the UI from json files after export from a UI Editor that I am making using winapi.
