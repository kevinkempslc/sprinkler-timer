/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function initialize()
{
    var elem = document.getElementById('name1');
    elem.addEventListener('change', onZone1NameChange);
    elem = document.getElementById('name2');
    elem.addEventListener('change', onZone2NameChange);
    elem = document.getElementById('name3');
    elem.addEventListener('change', onZone3NameChange);
    elem = document.getElementById('name4');
    elem.addEventListener('change', onZone4NameChange);
    elem = document.getElementById('name5');
    elem.addEventListener('change', onZone5NameChange);
    elem = document.getElementById('name6');
    elem.addEventListener('change', onZone6NameChange);
    elem = document.getElementById('name7');
    elem.addEventListener('change', onZone7NameChange);
    elem = document.getElementById('name8');
    elem.addEventListener('change', onZone8NameChange);

    elem = document.getElementById('dec_zone1');
    elem.addEventListener('click', onDecZone1);
    elem = document.getElementById('dec_zone2');
    elem.addEventListener('click', onDecZone2);
    elem = document.getElementById('dec_zone3');
    elem.addEventListener('click', onDecZone3);
    elem = document.getElementById('dec_zone4');
    elem.addEventListener('click', onDecZone4);
    elem = document.getElementById('dec_zone5');
    elem.addEventListener('click', onDecZone5);
    elem = document.getElementById('dec_zone6');
    elem.addEventListener('click', onDecZone6);
    elem = document.getElementById('dec_zone7');
    elem.addEventListener('click', onDecZone7);
    elem = document.getElementById('dec_zone8');
    elem.addEventListener('click', onDecZone8);
    elem = document.getElementById('inc_zone1');
    elem.addEventListener('click', onIncZone1);
    elem = document.getElementById('inc_zone2');
    elem.addEventListener('click', onIncZone2);
    elem = document.getElementById('inc_zone3');
    elem.addEventListener('click', onIncZone3);
    elem = document.getElementById('inc_zone4');
    elem.addEventListener('click', onIncZone4);
    elem = document.getElementById('inc_zone5');
    elem.addEventListener('click', onIncZone5);
    elem = document.getElementById('inc_zone6');
    elem.addEventListener('click', onIncZone6);
    elem = document.getElementById('inc_zone7');
    elem.addEventListener('click', onIncZone7);
    elem = document.getElementById('inc_zone8');
    elem.addEventListener('click', onIncZone8);

    updateFooter();
    getDataReturnXml(commandURL+"sendall", handleBulkData);
}

function onZone1NameChange()
{
    var elem = document.getElementById('name1');
    var name = elem.value;
    var params = "zone=0&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone2NameChange()
{
    var elem = document.getElementById('name2');
    var name = elem.value;
    var params = "zone=1&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone3NameChange()
{
    var elem = document.getElementById('name3');
    var name = elem.value;
    var params = "zone=2&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone4NameChange()
{
    var elem = document.getElementById('name4');
    var name = elem.value;
    var params = "zone=3&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone5NameChange()
{
    var elem = document.getElementById('name5');
    var name = elem.value;
    var params = "zone=4&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone6NameChange()
{
    var elem = document.getElementById('name6');
    var name = elem.value;
    var params = "zone=5&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone7NameChange()
{
    var elem = document.getElementById('name7');
    var name = elem.value;
    var params = "zone=6&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onZone8NameChange()
{
    var elem = document.getElementById('name8');
    var name = elem.value;
    var params = "zone=7&name="+name;
    getDataReturnText(commandURL+"zonename&" + params, nullHandler);
}

function onDecZone1()
{
    decrementZone(0, 'duration1');
}

function onDecZone2()
{
    decrementZone(1, 'duration2');
}

function onDecZone3()
{
    decrementZone(2, 'duration3');
}

function onDecZone4()
{
    decrementZone(3, 'duration4');
}

function onDecZone5()
{
    decrementZone(4, 'duration5');
}

function onDecZone6()
{
    decrementZone(5, 'duration6');
}

function onDecZone7()
{
    decrementZone(6, 'duration7');
}

function onDecZone8()
{
    decrementZone(7, 'duration8');
}

function onIncZone1()
{
    incrementZone(0, 'duration1');
}

function onIncZone2()
{
    incrementZone(1, 'duration2');
}

function onIncZone3()
{
    incrementZone(2, 'duration3');
}

function onIncZone4()
{
    incrementZone(3, 'duration4');
}

function onIncZone5()
{
    incrementZone(4, 'duration5');
}

function onIncZone6()
{
    incrementZone(5, 'duration6');
}

function onIncZone7()
{
    incrementZone(6, 'duration7');
}

function onIncZone8()
{
    incrementZone(7, 'duration8');
}

function decrementZone(zone, element)
{
    var elem = document.getElementById(element);
    var duration = parseInt(elem.value);

    if (duration === 0)
    {
        duration = 60;
    }
    else
    {
        duration--;
    }
    elem.value = duration;
    setZoneDuration(zone, duration);
}

function incrementZone(zone, element)
{
    var elem = document.getElementById(element);
    var duration = parseInt(elem.value);

    if (duration === 60)
        duration = 0;
    else
        duration++;
    elem.value = duration;
    setZoneDuration(zone, duration);
}

function setZoneDuration(zone, duration)
{
    var params = "zone=" + zone + "&duration="+duration;
    getDataReturnText(commandURL+"zoneduration&" + params, nullHandler);
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
    if (zones === null || zones.length === 0)
        return;
    zoneDuration = zones[0].getElementsByTagName("zoneDuration");
    zoneName = zones[0].getElementsByTagName("zoneName");
    
    
    // populate HTML
    zoneNames = document.getElementsByClassName("zone_name");
    zoneDurations = document.getElementsByClassName("zone_duration");
    
    for (i = 0; i < 8; i++)
    {
        zoneNames[i].value = zoneName[i].firstChild.data;
        zoneDurations[i].value = zoneDuration[i].firstChild.data;
    }
}

function handleUpdateData(xml)
{
    if (xml === null)
    {
         window.location = "index.html";    
         return;
    }
}

addEventListener("load", initialize);
