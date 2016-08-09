/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package servlets;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 *
 * @author kkemp
 */
@WebServlet(name = "LoginRequest", urlPatterns =
{
    "/LoginRequest"
})
public class LoginRequest extends HttpServlet
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

        ServiceClient client = new ServiceClient(configFile.getSprinklerDaemonHost(), configFile.getSprinklerDaemonPort());
        response.setContentType("text/html;charset=UTF-8");

        try
        {
            response.setHeader("Cache-Control", "no-cache");

            String password = request.getParameter("password");
            
            if (password != null && password.compareTo(configFile.getPassword()) == 0)
            {
                if (client.open())
                {
                    client.checkSession(true);
                    client.close();    
                    PrintWriter out = response.getWriter();
                    out.print("ok");
                    out.close();
                    
                    return;
                }
            }
            PrintWriter out = response.getWriter();
            out.println("error");
        }
        catch (IOException exc)
        {
            exc.printStackTrace();
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