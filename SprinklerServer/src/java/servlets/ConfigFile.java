/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package servlets;

import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.XMLEvent;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

public class ConfigFile
{
    private String configFile;
    private String password;
	private String sprinklerDaemonHost;
	private int sprinklerDaemonPort;

	public ConfigFile(String filename)
    {
        configFile = filename;
    }

    public String getPassword()
    {
        return password;
    }

	public String getSprinklerDaemonHost()
	{
		return sprinklerDaemonHost;
	}

	public int getSprinklerDaemonPort()
	{
		return sprinklerDaemonPort;
	}

    public void readConfig()
    {
        try
        {
            // First create a new XMLInputFactory
            XMLInputFactory inputFactory = XMLInputFactory.newInstance();

            // Setup a new eventReader
            InputStream in = new FileInputStream(configFile);
            XMLEventReader eventReader = inputFactory.createXMLEventReader(in);
            // Read the XML document
            while (eventReader.hasNext())
            {
                XMLEvent event = eventReader.nextEvent();

                if (event.isStartElement())
                {
                    if (event.asStartElement().getName().getLocalPart().equals("password"))
                    {
                        event = eventReader.nextEvent();
                        password = event.asCharacters().getData();
					}
					else if(event.asStartElement().getName().getLocalPart().equals("sprinklerDaemonHost"))
					{
						event = eventReader.nextEvent();
						sprinklerDaemonHost = event.asCharacters().getData();
					}
					else if(event.asStartElement().getName().getLocalPart().equals("sprinklerDaemonPort"))
					{
						event = eventReader.nextEvent();
						sprinklerDaemonPort = Integer.parseInt(event.asCharacters().getData());
					}
                }
            }
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        }
        catch (XMLStreamException e)
        {
            e.printStackTrace();
        }
    }
}
