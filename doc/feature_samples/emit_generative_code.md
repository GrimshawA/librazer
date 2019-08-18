meta generation exercise

from class GameController, write a GameControllerJS which acts as 
proxy for all methods with attribute [JS], effectively generating all needed code at compile time for the bindings.

GameControllerJS: class {
	
	emit on Reflect<GameController>::methods where m.hasAttribute("JS") 
	{
		function $m.name ()
		{
			base.$m.name ();
		}
	}

}