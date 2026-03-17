
function Lerp( t, a, b ) 
{
	return a + t * (b - a); 
}

function GetRandomFloatInRange( min, max )
{
	return min +(Math.random() * (max - min));
}

function GetRandomIntInRange( min, max )
{
	return Math.floor((min +(Math.random() * (max - min))) +0.5);
}

function SetImage( type, src )
{
	var elements = document.getElementsByName(type);
	for(var i = 0; i < elements.length; i++)
	{
		elements[i].src = src;
	}

}

function SwitchContent( name, index, min, max ) 
{
	for(var i = min; i <= max; i++)
	{
		if(i != index)
		{
			var elements = document.getElementsByName(name +"_" +i.toString());

			for(var o = 0; o < elements.length; o++) 
			{
				elements[o].style.display = "none";
				elements[o].style.visibility = "hidden";
			}
		}
		else
		{
			var elements = document.getElementsByName(name +"_" +i.toString());
			for(var o = 0; o < elements.length; o++) 
			{
				elements[o].style.display = "block";
				elements[o].style.visibility = "visible";
			}
		}
	}
}

function SwitchContentStyle( name, min, max, index, key, valueOn, valueOff )
{
	for(let i = min; i <= max; i++)
	{
		if(i != index)
		{
			const elements = document.getElementsByName(name +"_" +i.toString());

			for(let o = 0; o < elements.length; o++) 
			{
				elements[o].style[key] = valueOff;
			}
		}
		else
		{
			const elements = document.getElementsByName(name +"_" +i.toString());
			for(let o = 0; o < elements.length; o++) 
			{
				elements[o].style[key] = valueOn;
			}
		}
	}
}

function HideContent( hide ) 
{
	var elements = document.getElementsByName(hide);
	for(var i = 0; i < elements.length; i++) 
	{
		elements[i].style.display = "none";
		elements[i].style.visibility = "hidden";
	}
}

function ShowContent( show ) 
{
	var elements = document.getElementsByName(show);
	for(var i = 0; i < elements.length; i++) 
	{
		elements[i].style.display = "block";
		elements[i].style.visibility = "visible";
	}
}

function HideContentViaID( hide ) 
{
	var element = document.getElementById(hide);

	element.style.display = "none";
	element.style.visibility = "hidden";
}

function ShowContentViaID( show ) 
{
	var element = document.getElementById(show);

	element.style.display = "block";
	element.style.visibility = "visible";
}

function ToggleText( id, textA, textB )
{
	var elements = document.getElementsByName(id);
	for(var i = 0; i < elements.length; i++) 
	{
		if(elements[i].innerText == textA)
		{
			elements[i].innerText = textB;
		}
		else
		{
			elements[i].innerText = textA;
		}
	}
}

function ToggleStyle( id, key, valueOn, valueOff )
{
	var elements = document.getElementsByName(id);
	for(var i = 0; i < elements.length; i++) 
	{
		//console.log([elements[i].style[key], valueOff])

		if(elements[i].style[key] == valueOn)
		{
			elements[i].style[key] = valueOff;
		}
		else
		{
			elements[i].style[key] = valueOn;
		}
	}
}

function ForceText( id, state, textA, textB )
{
	var elements = document.getElementsByName(id);
	for(var i = 0; i < elements.length; i++) 
	{
		if(state == true)
		{
			elements[i].innerText = textA;
		}
		else
		{
			elements[i].innerText = textB;
		}
	}
}