/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package servlets;

import SprinklerServer.thrift.*;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import java.io.IOException;
import java.net.Socket;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author kkemp
 */
public class ServiceClient
{
    private Socket sock;
    final int NUMZONES = 8;
    final int NUMPROGRAMS = 4;

	protected SprinklerService.Client client;
	protected TTransport transport;

	protected String hostname;
	protected int port;
    
    final String runStates[] = { "Running", "Manual", "Rain", "Stopped" };

	public ServiceClient(String hostname, int port)
	{
		this.hostname = hostname;
		this.port = port;
	}

	public boolean open()
    {
		if(hostname == null || hostname.isEmpty())
			return false;

        try
        {
			Logger.getLogger(ServiceClient.class.getName()).log(Level.INFO, "Starting connection to " + hostname + ":" + port);
			transport = new TSocket(hostname, port);
			transport.open();
			TBinaryProtocol binaryProtocol = new TBinaryProtocol(transport);
			client = new SprinklerService.Client(binaryProtocol);
        }
		catch(TTransportException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, "Error on connect", e);
			return false;
		}

		return true;
    }
    
    public void close()
    {
		transport.close();
    }

    public void send(byte packet[])
    {
        try
        {
            sock.getOutputStream().write(packet);
        }
        catch (IOException ex)
        {
            Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public int read(byte[] buffer)
    {
        int numBytes = 0;

        try
        {
            numBytes = sock.getInputStream().read(buffer);
        }
        catch (IOException ex)
        {
            Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, ex);
        }
        return numBytes;
    }
    
    public void runTimer()
    {
		try
		{
			client.runTimer();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
		}
    }
    
    public String shutdown()
    {
		boolean success;
		try
		{
			success = client.shutdown();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

		if(success)
			return "<html><body>Timer has been shutdown.</body></html>";
        
        return "error";
    }
    
    public boolean checkSession(boolean beginSession)
    {
		try
		{
			return client.checkSession(beginSession);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			return false;
		}
    }

    public String runZone(int zone)
    {
		boolean running;
		try
		{
			running = client.runZone(zone);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			return "error";
		}

		if(!running)
			return "Stopped";
		else
			return "Running";
    }
    
    public String runProgram(int program)
    {
		boolean running;
		try
		{
			running = client.runProgram(program);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			return "error";
		}

		if (!running)
			return "Stopped";
		else
			return "Running";
    }
    
    private String processUpdateData(UpdateData data)
    {
		if(data == null)
			return "error";

        StringBuilder sb = new StringBuilder();
		sb.append("<updatedata>\n");
		sb.append("<params>\n");
		sb.append(" <hour>");
		sb.append(Integer.toString(data.getHour()));
		sb.append("</hour>\n");
		sb.append(" <minute>");
		sb.append(Integer.toString(data.getMinute()));
		sb.append("</minute>\n");
		sb.append(" <activeProgram>");
		sb.append(Integer.toString(data.getActiveProgram()));
		sb.append("</activeProgram>\n");
		sb.append(" <activeZone>");
		sb.append(Integer.toString(data.getActiveZone()));
		sb.append("</activeZone>\n");
		sb.append(" <manualProgram>");
		sb.append(Integer.toString(data.getManualProgram()));
		sb.append("</manualProgram>\n");
		sb.append(" <manualZone>");
		sb.append(Integer.toString(data.getManualZone()));
		sb.append("</manualZone>\n");
		sb.append(" <runState>");
		// Translates RunState's 1-based values to 0-based indexes for nice output
		sb.append(runStates[data.getRunState().getValue() - 1]);
		sb.append("</runState>\n");
		sb.append(" <overlap>");
		sb.append((data.isStartTimeOverlap()) ? "yes" : "no");
		sb.append("</overlap>\n");
		sb.append("</params>\n");
		sb.append("</updatedata>\n");

		return sb.toString();
    }
    
    private String processBulkData(BulkData data)
    {
		if(data == null)
			return "error";

        StringBuilder sb = new StringBuilder();
		List<Day> programDays = data.getProgramDays();

		sb.append("<bulkdata>\n");
		sb.append("<params>\n");
		sb.append(" <runState>");
		if(data.getRunState() == RunState.RS_AUTO)
			sb.append("Running");
		else if(data.getRunState() == RunState.RS_MANUAL)
			sb.append("Manual");
		else
			sb.append("Stopped");
		sb.append("</runState>\n");
		sb.append(" <overlap>");
		sb.append((data.isStartTimeOverlap()) ? "yes" : "no");
		sb.append("</overlap>\n");
		sb.append(" <currentProgram>");
		// Programs from the timer daemon are 0-based indexes
		sb.append(Integer.toString(data.getCurrentProgram() + 1));
		sb.append("</currentProgram>\n");
		sb.append(" <startHour>");
		sb.append(Integer.toString(data.getStartHour()));
		sb.append("</startHour>\n");
		sb.append(" <startMin>");
		sb.append(Integer.toString(data.getStartMin()));
		sb.append("</startMin>\n");
		sb.append(" <programMode>");
		// Note: The ajax javascript is expecting this of the form:
		// ProgramMode.DAY_OF_WEEK == 0
  		// ProgramMode.EVEN_DAYS == 1
  		// ProgramMode.ODD_DAYS == 2
  		// ProgramMode.DAY_INTERVAL == 3
		sb.append(Integer.toString(data.getProgramMode().getValue() - 1));
		sb.append("</programMode>\n");
		sb.append(" <programInterval>");
		sb.append(Integer.toString(data.getProgramInterval()));
		sb.append("</programInterval>\n");
		sb.append(" <sunday>");
		sb.append( (programDays.contains(Day.SUNDAY)) ? "yes":"no");
		sb.append("</sunday>\n");
		sb.append(" <monday>");
		sb.append( (programDays.contains(Day.MONDAY)) ? "yes":"no");
		sb.append("</monday>\n");
		sb.append(" <tuesday>");
		sb.append( (programDays.contains(Day.TUESDAY)) ? "yes":"no");
		sb.append("</tuesday>\n");
		sb.append(" <wednesday>");
		sb.append( (programDays.contains(Day.WEDNESDAY)) ? "yes":"no");
		sb.append("</wednesday>\n");
		sb.append(" <thursday>");
		sb.append( (programDays.contains(Day.THURSDAY)) ? "yes":"no");
		sb.append("</thursday>");
		sb.append(" <friday>");
		sb.append( (programDays.contains(Day.FRIDAY)) ? "yes":"no");
		sb.append("</friday>\n");
		sb.append(" <saturday>");
		sb.append( (programDays.contains(Day.SATURDAY)) ? "yes":"no");
		sb.append("</saturday>\n");

		// Note: This is assuming that the timer daemon sends us back 0-indexed
		// zone numbers. If not, it will need to be adjusted.
		// TODO: Verify that the daemon sends back 0-indexed numbers
		StringBuilder zonesBuilder = new StringBuilder();
		for(int i = 0; i < NUMZONES; i++)
		{
			Zone currentZone = data.getProgramZones().get(i);
			sb.append(" <programZone");
			sb.append(i + 1);
			sb.append(">");
			if(currentZone.isEnabledForCurrentProgram())
				sb.append("yes");
			else
				sb.append("no");
			sb.append("</programZone");
			sb.append(i + 1);
			sb.append(">\n");

			zonesBuilder.append("  <zoneDuration>");
			zonesBuilder.append(Integer.toString(currentZone.getDuration()));
			zonesBuilder.append("</zoneDuration>\n");
			zonesBuilder.append("  <zoneName>");
			zonesBuilder.append(currentZone.getName());
			zonesBuilder.append("</zoneName>\n");
		}

		sb.append(" <zones>\n");
		sb.append(zonesBuilder.toString());
		sb.append(" </zones>\n");

		sb.append("</params>\n");
		sb.append("</bulkdata>\n");

		return sb.toString();
    }
    
    public String sendAll()
    {
		BulkData data = null;
		try
		{
			data = client.sendAll();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
		}
        
        return processBulkData(data);
    }
    
    public String sendUpdate()
    {
		UpdateData data = null;
		try
		{
			data = client.sendUpdate();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
		}
        
        return processUpdateData(data);
    }
    
   	public String nextProgram()
    {
		BulkData data = null;
		try
		{
			data = client.selectProgram();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
		}

        // read response
        return processBulkData(data);
    }
    
    public String daysMode()
    {
		boolean success;
		try
		{
			success = client.selectDayMode();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
			return "error";
    }

    public String evenMode()
    {
		boolean success;
		try
		{
			success = client.selectEvenMode();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String oddMode()
    {
		boolean success;
		try
		{
			success = client.selectOddMode();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String intervalMode()
    {
        boolean success;
		try
		{
			success = client.selectIntervalMode();
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String setInterval(int interval)
    {
		boolean success;
		try
		{
			success = client.setInterval(interval);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String setDays(List<Day> days)
    {
		boolean success;
		try
		{
			success = client.setDays(days);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String setZones(List<Integer> zones)
    {
		boolean success;
		try
		{
			success = client.setZones(zones);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        if(success)
            return "ok";
        else
        	return "error";
    }

    public String zoneName(int zoneIndex, String name)
    {
		boolean success;
		try
		{
			success = client.setZoneName(zoneIndex, name);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String zoneDuration(int zoneIndex, int duration)
    {
		boolean success;
		try
		{
			success = client.setZoneDuration(zoneIndex, duration);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}
        
        // read response
        if(success)
            return "ok";
        else
        	return "error";
    }

    public String startTime(int hours, int minutes)
    {
		boolean success;
		try
		{
			success = client.setProgramStartTime(hours, minutes);
		}
		catch(TException e)
		{
			Logger.getLogger(ServiceClient.class.getName()).log(Level.SEVERE, null, e);
			success = false;
		}

		String r;
        // read response
		if (success)
			r = "ok";
		else
			r = "error";
        
        StringBuilder sb = new StringBuilder();
        sb.append("<starttimestatus>\n");
        sb.append("<params>\n");
        sb.append(" <status>");
        sb.append(r);
        sb.append("</status>\n");
        sb.append("</params>\n");
        sb.append("</starttimestatus>\n");
        
        return sb.toString();
    }
}
