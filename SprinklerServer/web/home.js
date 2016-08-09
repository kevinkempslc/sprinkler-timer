/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function initialize()
{
     try
    {
        var elem = document.getElementById('run_button');
        elem.addEventListener('click', onRunClick);
        elem = document.getElementById('program_button');
        elem.addEventListener('click', onProgramClick);
        elem = document.getElementById('start_hour');
        elem.addEventListener('change', onStartTimeChange);
        elem = document.getElementById('dec_hour');
        elem.addEventListener('click', onDecHour);
        elem = document.getElementById('inc_hour');
        elem.addEventListener('click', onIncHour);
        elem = document.getElementById('dec_minute');
        elem.addEventListener('click', onDecMinute);
        elem = document.getElementById('inc_minute');
        elem.addEventListener('click', onIncMinute);
        elem = document.getElementById('dec_interval');
        elem.addEventListener('click', onDecInterval);
        elem = document.getElementById('inc_interval');
        elem.addEventListener('click', onIncInterval);
        elem = document.getElementById('dayofweek');
        elem.addEventListener('click', onDayOfWeekButtonClick);
        elem = document.getElementById('evendays');
        elem.addEventListener('click', onEvenButtonClick);
        elem = document.getElementById('odddays');
        elem.addEventListener('click', onOddButtonClick);
        elem = document.getElementById('intervaldays');
        elem.addEventListener('click', onIntervalButtonClick);
        elem = document.getElementById('sunday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('monday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('tuesday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('wednesday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('thursday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('friday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('saturday');
        elem.addEventListener('click', onDayButtonClick);
        elem = document.getElementById('zone1');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone2');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone3');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone4');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone5');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone6');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone7');
        elem.addEventListener('click', onZoneButtonClick);
        elem = document.getElementById('zone8');
        elem.addEventListener('click', onZoneButtonClick);
    }
    catch (err)
    {
        alert("There was an exception in initialize() while setting listeners: " + err.message);
    }
    updateFooter();
    getDataReturnXml(commandURL + "sendall", handleBulkData);
}

function onDecHour()
{
    var hours = document.getElementById("start_hour");
    var h = parseInt(hours.value);
    
    if (h > 0)
        h--;
    else
        h = 23;
    hours.value = h;
    onStartTimeChange();
}

function onIncHour()
{
    var hours = document.getElementById("start_hour");
    var h = parseInt(hours.value);

    if (h === 23)
        h = 0;
    else
        h++;
    hours.value = h;
    onStartTimeChange();
}

function onDecMinute()
{
    var minutes = document.getElementById("start_minute");
    var m = parseInt(minutes.value);
    
    if (m > 0)
    {
        m-=5;
    }
    else
        m = 55;
    minutes.value = m;
    onStartTimeChange();
}

function onIncMinute()
{
    var minutes = document.getElementById("start_minute");
    var m = parseInt(minutes.value);
    if (m === 55)
        m = 0;
    else
    {
        m+=5;
    }
    minutes.value = m;
    onStartTimeChange();
}

function onDecInterval()
{
    var interval = document.getElementById("interval");
    var i = parseInt(interval.value);
    
    if (i > 1)
        i--;
    else
        i = 7;
    interval.value = i;
    onIntervalChange();
}

function onIncInterval()
{
    var interval = document.getElementById("interval");
    var i = parseInt(interval.value);

    if (i === 7)
        i = 1;
    else
        i++;
    interval.value = i;
    onIntervalChange();
}

function onIntervalChange()
{
    var days = document.getElementById("interval").value;
    getDataReturnText(commandURL + "interval&" + "days=" + days, genericHandler);
}

function onRunClick()
{
    sendCommand("run", genericHandler);
}

function onProgramClick()
{
    getDataReturnXml(commandURL + "program", handleBulkData);
}

function onStartTimeChange()
{
    var hours = document.getElementById("start_hour").value;
    var minutes = document.getElementById("start_minute").value;
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
    {
        alert("Invalid duration value.");
    }
    else
        getDataReturnXml(commandURL + "starttime&" + "hours=" + hours + "&minutes=" + minutes, onStartTimeStatus);
}

function onStartTimeStatus(xml)
{
    if (xml === null)
    {
         window.location = "index.html";    
         return;
    }
    try
    {
        var startTimeStatusElements = xml.getElementsByTagName("status");
        var status = startTimeStatusElements[0];
        var s = status.firstChild.data;
        var overlap = document.getElementById("overlap");
        overlap.innerHTML = (s !== "ok") ? "Overlap!" : "";
    }
    catch (err)
    {
        alert("Exception in onStartTimeStatus(): " + err.message);
    }
}

function onDayOfWeekButtonClick()
{
    sendCommand("daysmode", genericHandler);
    handleDayButtonClick(0);
}

function onEvenButtonClick()
{
    sendCommand("evenmode", genericHandler);
    handleDayButtonClick(1);
}

function onOddButtonClick()
{
    sendCommand("oddmode", genericHandler);
    handleDayButtonClick(2);
}

function onIntervalButtonClick()
{
    sendCommand("intervalmode", genericHandler);
    handleDayButtonClick(3);
}

function onDayButtonClick()
{
    var params = "sun=";
    var elem = document.getElementById('sunday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&mon=";
    var elem = document.getElementById('monday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&tues=";
    var elem = document.getElementById('tuesday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&wed=";
    var elem = document.getElementById('wednesday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&thurs=";
    var elem = document.getElementById('thursday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&fri=";
    var elem = document.getElementById('friday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&sat=";
    var elem = document.getElementById('saturday');
    if (elem.checked)
        params += "yes";
    else
        params += "no";
    getDataReturnText(commandURL + "daylist&" + params, genericHandler);
}

function handleDayButtonClick(daymode)
{
    var daylist = document.getElementById("daylist");
    daylist.style.display = (daymode === 0) ? "block" : "none";

    var intervalItem = document.getElementById("interval_item");
    intervalItem.style.display = (daymode === 3) ? "block" : "none";
}

function showRunState(text)
{
    var elem = document.getElementById('run_button');
    elem.style.color = "#5a6bbe";
    elem.innerHTML = text;
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
        var runStateElements = xml.getElementsByTagName("runState");
        var runState = runStateElements[0].firstChild.data;
        
        var overlapElements = xml.getElementsByTagName("overlap");
        var overlap = overlapElements[0].firstChild.data;

        var runButton = document.getElementById("run_button");
        runButton.innerHTML = runState;
        if (runState === "Manual")
            runButton.style.color = "grey";
        else
            runButton.style.color = "#5a6bbe";
        document.getElementById("overlap").innerHTML = (overlap === "yes" ? "Overlap!" : "");
}
    catch (err)
    {
        alert("Exception in handleUpdateData(): " + err.message);
    }
}

function handleBulkData(xml)
{
    if (xml === null)
    {
         window.location = "index.html";    
         return;
    }
    
    try
    {
        // parse XML
        var overlapElements = xml.getElementsByTagName("overlap");
        overlap = overlapElements[0].firstChild.data;
        runState = xml.getElementsByTagName("runState")[0].firstChild.data;
        currentProgram = xml.getElementsByTagName("currentProgram")[0].firstChild.data;
        startHour = xml.getElementsByTagName("startHour")[0].firstChild.data;
        startMin = xml.getElementsByTagName("startMin")[0].firstChild.data;
        programMode = xml.getElementsByTagName("programMode")[0].firstChild.data;
        programInterval = xml.getElementsByTagName("programInterval")[0].firstChild.data;
        sunday = xml.getElementsByTagName("sunday")[0].firstChild.data === "yes";
        monday = xml.getElementsByTagName("monday")[0].firstChild.data === "yes";
        tuesday = xml.getElementsByTagName("tuesday")[0].firstChild.data === "yes";
        wednesday = xml.getElementsByTagName("wednesday")[0].firstChild.data === "yes";
        thursday = xml.getElementsByTagName("thursday")[0].firstChild.data === "yes";
        friday = xml.getElementsByTagName("friday")[0].firstChild.data === "yes";
        saturday = xml.getElementsByTagName("saturday")[0].firstChild.data === "yes";

        var programZone = new Array();
        programZone[0] = xml.getElementsByTagName("programZone1")[0].firstChild.data;
        programZone[1] = xml.getElementsByTagName("programZone2")[0].firstChild.data;
        programZone[2] = xml.getElementsByTagName("programZone3")[0].firstChild.data;
        programZone[3] = xml.getElementsByTagName("programZone4")[0].firstChild.data;
        programZone[4] = xml.getElementsByTagName("programZone5")[0].firstChild.data;
        programZone[5] = xml.getElementsByTagName("programZone6")[0].firstChild.data;
        programZone[6] = xml.getElementsByTagName("programZone7")[0].firstChild.data;
        programZone[7] = xml.getElementsByTagName("programZone8")[0].firstChild.data;

        zones = xml.getElementsByTagName("zones");
        zoneEnabled = zones[0].getElementsByTagName("zoneEnabled");
        zoneName = zones[0].getElementsByTagName("zoneName");


        // populate HTML
        document.getElementById("program_button").innerHTML = currentProgram;
        document.getElementById("run_button").innerHTML = runState;
        document.getElementById("start_hour").value = startHour;
        document.getElementById("start_minute").value = startMin;
        document.getElementById("overlap").innerHTML = (overlap === "yes" ? "Overlap!" : "");

        switch (programMode)
        {
            case "0":
                document.getElementById("dayofweek").checked = true;
                handleDayButtonClick(0);
                break;
            case "1":
                document.getElementById("evendays").checked = true;
                handleDayButtonClick(1);
                break;
            case "2":
                document.getElementById("odddays").checked = true;
                handleDayButtonClick(2);
                break;
            case "3":
                document.getElementById("intervaldays").checked = true;
                handleDayButtonClick(3);
                break;
        }
    }
    catch (err)
    {
        alert("There was an exception in handleBulkData(): " + err.message);
    }
    try
    {
        document.getElementById("sunday").checked = sunday;
        document.getElementById("monday").checked = monday;
        document.getElementById("tuesday").checked = tuesday;
        document.getElementById("wednesday").checked = wednesday;
        document.getElementById("thursday").checked = thursday;
        document.getElementById("friday").checked = friday;
        document.getElementById("saturday").checked = saturday;
    }
    catch (err)
    {
        alert("There was an exception in handleBulkData() checking day buttons: " + err.message);
    }
    try
    {
        document.getElementById("interval").value = programInterval;
    }
    catch (err)
    {
        alert("There was an exception in handleBulkData() setting interval:" + err.message);
    }
    try
    {
        zoneLabels = document.getElementsByClassName("zone_label");
        zoneButtons = document.getElementsByClassName("zonecb");
        
        if (zoneLabels === null)
        {
            alert("unable to get items of class zone_label.")
            return;
        }
        if (zoneButtons === null)
        {
            alert("unable to get items of class zonecb.")
            return;
        }
        for (i = 0; i < 8; i++)
        {
            zoneLabels[i].innerHTML = zoneName[i].firstChild.data;
            zoneButtons[i].checked = programZone[i] === "yes";
        }
    }
    catch (err)
    {
        alert("There was an exception in handleBulkData() setting zone data" + err.message);
    }
}

function onZoneButtonClick()
{
    var params = "zone1=";
    var elem = document.getElementById('zone1');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone2=";
    var elem = document.getElementById('zone2');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone3=";
    var elem = document.getElementById('zone3');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone4=";
    var elem = document.getElementById('zone4');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone5=";
    var elem = document.getElementById('zone5');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone6=";
    var elem = document.getElementById('zone6');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone7=";
    var elem = document.getElementById('zone7');
    if (elem.checked)
        params += "yes";
    else
        params += "no";

    params += "&zone8=";
    var elem = document.getElementById('zone8');
    if (elem.checked)
        params += "yes";
    else
        params += "no";
    getDataReturnText(commandURL + "zones&" + params, genericHandler);
}

addEventListener("load", initialize);

