/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package servlets;

import SprinklerServer.thrift.Day;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author kkemp
 */
@WebServlet(name = "ExecuteCommand", urlPatterns =
{
    "/ExecuteCommand"
})
public class ExecuteCommand extends HttpServlet
{

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException
    {
		String url = getServletContext().getRealPath("WEB-INF/config.xml");
		ConfigFile configFile = new ConfigFile(url);
		configFile.readConfig();

        String command = request.getParameter("command");
        
        ServiceClient client = new ServiceClient(configFile.getSprinklerDaemonHost(), configFile.getSprinklerDaemonPort());
        String responseText = "";
        String responseXml = "";

        if (client.open())
        {
            if (command.compareToIgnoreCase("run") == 0)
            {
                client.runTimer();
                responseText = "";
            }
            else if (command.compareToIgnoreCase("shutdown") == 0)
                responseText = client.shutdown();
            else if (command.compareToIgnoreCase("program") == 0)
                responseXml = client.nextProgram();
            else if (command.compareToIgnoreCase("sendall") == 0)
                responseXml = client.sendAll();
            else if (command.compareToIgnoreCase("sendupdate") == 0)
                responseXml = client.sendUpdate();
            else if (command.compareToIgnoreCase("runzone") == 0)
                responseText = client.runZone(Integer.parseInt(request.getParameter("zone")));
            else if (command.compareToIgnoreCase("runprogram") == 0)
                responseText = client.runProgram(Integer.parseInt(request.getParameter("program")));
            else if (command.compareToIgnoreCase("daysmode") == 0)
                responseText = client.daysMode();
            else if (command.compareToIgnoreCase("evenmode") == 0)
                responseText = client.evenMode();
            else if (command.compareToIgnoreCase("oddmode") == 0)
                responseText = client.oddMode();
            else if (command.compareToIgnoreCase("intervalmode") == 0)
                responseText = client.intervalMode();
            else if (command.compareToIgnoreCase("zonename") == 0)
                responseText = client.zoneName(Integer.parseInt(request.getParameter("zone")), request.getParameter("name"));
            else if (command.compareToIgnoreCase("zoneduration") == 0)
                responseText = client.zoneDuration(Integer.parseInt(request.getParameter("zone")), Integer.parseInt(request.getParameter("duration")));
            else if (command.compareToIgnoreCase("starttime") == 0)
            {
                String p1 = request.getParameter("hours");
                String p2 = request.getParameter("minutes");
                int hours = Integer.parseInt(p1);
                int minutes = Integer.parseInt(p2);
                responseXml = client.startTime(hours, minutes);
            }
            else if (command.compareToIgnoreCase("interval") == 0)
                responseText = client.setInterval(Integer.parseInt(request.getParameter("days")));
            else if (command.compareToIgnoreCase("daylist") == 0)
            {
				List<Day> days = new ArrayList<Day>();
                if (request.getParameter("sun").compareToIgnoreCase("yes") == 0)
                    days.add(Day.SUNDAY);
                if (request.getParameter("mon").compareToIgnoreCase("yes") == 0)
                    days.add(Day.MONDAY);
                if (request.getParameter("tues").compareToIgnoreCase("yes") == 0)
                    days.add(Day.TUESDAY);
                if (request.getParameter("wed").compareToIgnoreCase("yes") == 0)
                    days.add(Day.WEDNESDAY);
                if (request.getParameter("thurs").compareToIgnoreCase("yes") == 0)
                    days.add(Day.THURSDAY);
                if (request.getParameter("fri").compareToIgnoreCase("yes") == 0)
                    days.add(Day.FRIDAY);
                if (request.getParameter("sat").compareToIgnoreCase("yes") == 0)
                    days.add(Day.SATURDAY);
                
                responseText = client.setDays(days);
            }
            else if (command.compareToIgnoreCase("zones") == 0)
            {
				List<Integer> zones = new ArrayList<Integer>();
                if (request.getParameter("zone1").compareToIgnoreCase("yes") == 0)
                    zones.add(1);
                if (request.getParameter("zone2").compareToIgnoreCase("yes") == 0)
                    zones.add(2);
                if (request.getParameter("zone3").compareToIgnoreCase("yes") == 0)
                    zones.add(3);
                if (request.getParameter("zone4").compareToIgnoreCase("yes") == 0)
                    zones.add(4);
                if (request.getParameter("zone5").compareToIgnoreCase("yes") == 0)
                    zones.add(5);
                if (request.getParameter("zone6").compareToIgnoreCase("yes") == 0)
                    zones.add(6);
                if (request.getParameter("zone7").compareToIgnoreCase("yes") == 0)
                    zones.add(7);
                if (request.getParameter("zone8").compareToIgnoreCase("yes") == 0)
                    zones.add(8);
                
                responseText = client.setZones(zones);
            }
            
            client.close();
        }
        
        if (responseText != null && responseText.length() > 0)
        {
            response.setContentType("text/html;charset=UTF-8");
            response.setHeader("Cache-Control", "no-cache");
            PrintWriter out = response.getWriter();
            try
            {
                out.println(responseText);
            }
            finally
            {
                out.close();
            }
        }
        else if (responseXml != null)
        {
            response.setContentType("text/xml");
            response.setHeader("Cache-Control", "no-cache");
            PrintWriter writer = response.getWriter();
            try
            {
                writer.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
                writer.print(responseXml);
            }
            finally
            {
                writer.close();
            }
        }
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException
    {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException
    {
        processRequest(request, response);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo()
    {
        return "Short description";
    }// </editor-fold>

}
