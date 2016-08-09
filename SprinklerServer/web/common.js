/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
var commandURL = "ExecuteCommand?command=";

function sendCommand(command, handler)
{
    getDataReturnText(commandURL + command, handler);
}

function genericHandler()
{

}

function nullHandler(xml)
{
}

function handleUpdateFooter(xml)
{
    if (xml === null)
        return;
    
    try
    {
        var hourElements = xml.getElementsByTagName("hour");
        hour = hourElements[0].firstChild.data;
        minute = xml.getElementsByTagName("minute")[0].firstChild.data;
        minute = pad(minute, 2);
        document.getElementById("currenttime").innerHTML = hour + ":" + minute;

        activeProgram = xml.getElementsByTagName("activeProgram")[0].firstChild.data;
        activeZone = xml.getElementsByTagName("activeZone")[0].firstChild.data;
        manualZone = xml.getElementsByTagName("manualZone")[0].firstChild.data;

        if (parseInt(activeProgram) !== -1)
        {
            var p = parseInt(activeProgram) + 1;
            var z = parseInt(activeZone) + 1;
            document.getElementById("statusbar").innerHTML = "Running Program: " + p + " Zone: " + z;
        }
        else if (parseInt(manualZone) !== -1)
        {
            var z = parseInt(manualZone) + 1;
            document.getElementById("statusbar").innerHTML = "Running Zone: " + z;
        }
        else
        { 
            document.getElementById("statusbar").innerHTML = "No Program Currently Running";
        }
    }
    catch (err)
    {
        alert("There was an exception in handleUpdateFooter(): " + err.message);
    }

    handleUpdateData(xml);  // Ihis function is provided by the individual pages.
}

function updateFooter() {
    getDataReturnXml(commandURL + "sendupdate", handleUpdateFooter);

    t = setTimeout(function() {
        updateFooter();
    }, 1000);
}
function pad(number, length) {

    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }

    return str;

}

