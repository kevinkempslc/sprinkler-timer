/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function initialize()
{
    var elem = document.getElementById('programbutton1');
    elem.addEventListener('click', onProgram1Clicked);
    elem = document.getElementById('programbutton2');
    elem.addEventListener('click', onProgram2Clicked);
    elem = document.getElementById('programbutton3');
    elem.addEventListener('click', onProgram3Clicked);
    elem = document.getElementById('programbutton4');
    elem.addEventListener('click', onProgram4Clicked);

    elem = document.getElementById('zonebutton1');
    elem.addEventListener('click', onZone1Clicked);
    elem = document.getElementById('zonebutton2');
    elem.addEventListener('click', onZone2Clicked);
    elem = document.getElementById('zonebutton3');
    elem.addEventListener('click', onZone3Clicked);
    elem = document.getElementById('zonebutton4');
    elem.addEventListener('click', onZone4Clicked);
    elem = document.getElementById('zonebutton5');
    elem.addEventListener('click', onZone5Clicked);
    elem = document.getElementById('zonebutton6');
    elem.addEventListener('click', onZone6Clicked);
    elem = document.getElementById('zonebutton7');
    elem.addEventListener('click', onZone7Clicked);
    elem = document.getElementById('zonebutton8');
    elem.addEventListener('click', onZone8Clicked);
    
    updateFooter();
    getDataReturnXml(commandURL + "sendall", handleBulkData);
}

function onProgram1Clicked()
{
    getDataReturnText(commandURL + "runprogram&program=0", nullHandler);
}

function onProgram2Clicked()
{
    getDataReturnText(commandURL + "runprogram&program=1", nullHandler);
}

function onProgram3Clicked()
{
    getDataReturnText(commandURL + "runprogram&program=2", nullHandler);
}

function onProgram4Clicked()
{
    getDataReturnText(commandURL + "runprogram&program=3", nullHandler);
}

function onZone1Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=0", nullHandler);
}

function onZone2Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=1", nullHandler);
}

function onZone3Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=2", nullHandler);
}

function onZone4Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=3", nullHandler);
}

function onZone5Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=4", nullHandler);
}

function onZone6Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=5", nullHandler);
}

function onZone7Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=6", nullHandler);
}

function onZone8Clicked()
{
    getDataReturnText(commandURL + "runzone&zone=7", nullHandler);
}

function handleBulkData(xml)
{
    if (xml === null)
    {
         window.location = "index.html";    
         return;
    }
    
    // parse XML
    zones = xml.getElementsByTagName("zones");
    zoneName = zones[0].getElementsByTagName("zoneName");

    // populate HTML
    zoneNames = document.getElementsByClassName("zone_name");

    for (i = 0; i < 8; i++)
    {
        zoneNames[i].innerHTML = zoneName[i].firstChild.data;
    }
}

function handleUpdateData(xml)
{
    if (xml === null)
    {
         window.location = "index.html";    
         return;
    }
    
    try
    {
        var programElements = xml.getElementsByTagName("manualProgram");
        var program = programElements[0].firstChild.data;

        var zoneElements = xml.getElementsByTagName("manualZone");
        var zone = zoneElements[0].firstChild.data;

        program = parseInt(program);
        zone = parseInt(zone);

        var zoneButtons = document.getElementsByClassName("zone_button");
        var programButtons = document.getElementsByClassName("program_button");

        for (i = 0; i < 8; i++)
        {
            zoneButtons[i].innerHTML = "";
        }
        if (zone !== -1)
        {
            zoneButtons[zone].innerHTML = "On";
        }
        
            for (i = 0; i < 4; i++)
                programButtons[i].innerHTML = "";
        if (program !== -1)
        {
            programButtons[program].innerHTML = "On";
        }
    }
    catch (err)
    {
        alert("Exception in handleUpdateData(): " + err.message);
    }
}

addEventListener("load", initialize);
    