/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function initialize()
{
    try
    {
        var elem = document.getElementById('button1');
        elem.addEventListener('click', button1Click);
        elem = document.getElementById('button2');
        elem.addEventListener('click', button2Click);
        elem = document.getElementById('button3');
        elem.addEventListener('click', button3Click);
        elem = document.getElementById('button4');
        elem.addEventListener('click', button4Click);
        elem = document.getElementById('button5');
        elem.addEventListener('click', button5Click);
        elem = document.getElementById('button6');
        elem.addEventListener('click', button6Click);
        elem = document.getElementById('button7');
        elem.addEventListener('click', button7Click);
        elem = document.getElementById('button8');
        elem.addEventListener('click', button8Click);
        elem = document.getElementById('button9');
        elem.addEventListener('click', button9Click);
        elem = document.getElementById('button0');
        elem.addEventListener('click', button0Click);
        elem = document.getElementById('buttonClear');
        elem.addEventListener('click', buttonClearClick);
        elem = document.getElementById('buttonSubmit');
        elem.addEventListener('click', buttonSubmitClick);
        
        clearPassword();
    }
    catch (err)
    {
        alert("There was an exception in initialize() while setting listeners: " + err.message);
    }
}
    
function clearPassword()
{
    document.getElementById("pwDisplay").value = "";
}

function appendCharacter(ch)
{
    document.getElementById("pwDisplay").value += ch;
}

function button1Click()
{
   appendCharacter("1");
}

function button2Click()
{
   appendCharacter("2");
}

function button3Click()
{
   appendCharacter("3");
}

function button4Click()
{
   appendCharacter("4");
}

function button5Click()
{
   appendCharacter("5");
}

function button6Click()
{
   appendCharacter("6");
}

function button7Click()
{
   appendCharacter("7");
}

function button8Click()
{
   appendCharacter("8");
}

function button9Click()
{
   appendCharacter("9");
}

function button0Click()
{
   appendCharacter("0");
}

function buttonClearClick()
{
    clearPassword();
}

function buttonSubmitClick()
{
    getDataReturnText("LoginRequest?password=" + document.getElementById("pwDisplay").value, loginHandler);
    clearPassword();
}

function loginHandler(text)
{
    if (text === "ok")
        window.location = "home.html";
    else
    {
        document.getElementById("errorMessage").innerHTML = text;
    }
}
addEventListener("load", initialize);
   
    