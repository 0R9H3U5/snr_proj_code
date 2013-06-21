/*
 *	CIS457 Project 2 - Mini HTTP Server
 *	Authors: Brody Bersen, Eric Born, Schuyler Burson, Steven Diemer
 */

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.Hashtable;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.String;
import java.io.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.Date;
import java.util.TimeZone;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.text.ParseException;
import java.util.StringTokenizer;
import java.io.File;
import java.lang.Thread;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.*;

/**
 *	A simplified HTTP server.
 */
class testsvr
{
    public static void main(String args[])
    {
        ServerSocket serverSocket = null;
        Socket clientSocket = null;
        int connects = 0;
        int portSelect;
        String arguments[];
        boolean log = false;
        String documentRoot;
        BufferedWriter logfile = null;
        String logFileName = "";

        //parse the specified arguments
        arguments = parseArgs(args);
        
        //set the port
        if(arguments[0].equals("-1"))
        {
            portSelect = 44444;
        }
        else
        {
        	try {
            	portSelect = Integer.parseInt(arguments[0]);
            	if((portSelect < 0) || (portSelect > 65534))
            	{
            		System.out.println("Invalid port specified, setting to default (8080)");
            		portSelect = 44444;
            	}
            } catch (NumberFormatException e) {
            	System.out.println("Invalid port specified, setting to default (8080)");
            	portSelect = 44444;
            }
        }

        //set the docroot
        if(arguments[1].equals("-1"))
        {
            documentRoot = System.getProperty("user.dir");
        }
        else
        {
        	File dir = new File(arguments[1]);
        	if(dir.exists() && dir.isDirectory())
        	{
            	documentRoot = arguments[1];
            }
            else
            {
            	System.out.println("Specified document root (" + arguments[1] + ") is not a directory or does not exist.\n --> Setting document root to default location.");
            	documentRoot = System.getProperty("user.dir");
            }
        }

        System.out.println("Document Root set to: "+documentRoot);

        //set log file if necessary
        if(!arguments[2].equals("-1")) 
        {
            try 
            {
            	//create log directory if none exists
            	File log_dir = new File(System.getProperty("user.dir") + "\\log");
            	if (!log_dir.exists()) 
            	{
            		log_dir.mkdir();
            	}

            	logFileName = arguments[2];

            	//add file extension if not provided
            	if(logFileName.lastIndexOf(".") == -1)
            	{
            		logFileName += ".txt";
            	}
                FileWriter fstream = new FileWriter(log_dir + "\\" + logFileName);
                logfile = new BufferedWriter(fstream);

                logfile.write("Server started with document root: " + documentRoot + "\nListening on port: " + portSelect + "\n\n");
                log = true;
                logfile.close();
            } 
            catch(IOException e) 
            {
                System.err.println("Exception Caught: " + e.getMessage());
            }
        }

        try
        {
            // Create the server socket
            serverSocket = new ServerSocket(portSelect, 5);
			
            while(true)
			{
				// Wait for a connection
				try
				{
					clientSocket = serverSocket.accept();
					connects++;
				
					Thread t = new Thread(new ServiceClient(clientSocket, documentRoot, logFileName, log));
					t.start();
				}
				catch(IOException e)
				{
					System.exit(1);
				}
				
            }
        }
        catch (IOException ioe)
        {
            System.out.println("Error in SimpleWebServer: " + ioe);
			System.exit(1);
        }
    }

    /**
     * Parse the command line arguments
     *
     * @param args[] The array of command line arguments given.
     * @return String[] A sorted array of the args given and their params.
     */
    public static String[] parseArgs(String args[]) 
    {
        int i;
        int arg;

        String inputs[] =  {"-1", "-1", "-1"};

        for(i = 0; i < args.length; i++) 
        {
            if(args[i].equals("-p"))
            {
                arg = 0;
            }
            else if(args[i].equals("-docroot"))
            {
                arg = 1;
            }
            else if(args[i].equals("-logfile"))
            {
                arg = 2;
            }
            else
            {
                arg = 10;
            }

            try {
            	switch(arg) 
            	{
            	    case 0:
            	        inputs[0] = args[i+1];
            	        break;
            	    case 1:
            	        inputs[1] = args[i+1].replace("/", "\\");
            	        break;
            	    case 2:
            	        inputs[2] = args[i+1];
            	        break;
            	    default:
            	        break;   
            	}
            } catch (ArrayIndexOutOfBoundsException e) {
            	if(arg == 0)
            		System.out.println("No port specified, setting to default (8080)");
            	else if(arg == 1)
            		System.out.println("No document root specified, setting to default");
            	else if(arg == 2)
            		System.out.println("No logfile specified, no log is being recorded");
            }
        }

        return inputs;
	}
}

/**
 * The ServiceClient class handles all the request from the user and ensures that the
 * proper headers and data are returned.
 */
class ServiceClient implements Runnable
{	
	Socket connectionSocket = null;
	BufferedReader inbound = null;
	DataOutputStream outbound = null;
	String docRoot = null;
	String log_file = null;
	boolean logging_enabled = false;
	
	/**
	 *	Default Constructor
	 *
	 *	@param client The Socket connetion with the client.
	 * 	@param root The path to the document root.
	 *  @param logfile The name of the logfile to write to.
	 *	@param log Flag indicated if logging is enabled.
	 */
	ServiceClient(Socket client, String root, String logfile, boolean log)
	{
		connectionSocket = client;

		try
		{
			// set persistent connection timeout time
			connectionSocket.setSoTimeout(20000);

			inbound = new BufferedReader(new InputStreamReader( connectionSocket.getInputStream()));
            outbound = new DataOutputStream( connectionSocket.getOutputStream());
            docRoot = root;
            logging_enabled = log;
            log_file = logfile;
		}
		catch(IOException e)
		{
			System.exit(1);
		}
	}
	
	/**
	 *	Runs the basic operation of the ServerClient thread.
	 */
	public void run()
	{
		// set the default status code
		int statusCode = 200;
	
		String inputLine = "";
		String inputHeader = "";
		String method = "";
		String fileName = "";
		
		//These strings are sent back as the server response.
		String statusString = "HTTP/1.1 200 OK" + "\r\n";
		String serverString = "Server: Java HTTPServer";
		String lengthString = null;
		String dateString = null;
		String lastModifiedString = null;
		String contentTypeString = null;
		String ifModSince = null;

		try
		{
			//grab the first line of the request header
			inputLine = inbound.readLine();

			inputHeader += "\n\t" + inputLine;
			// parse the header for the If-Modified-Since and Connection header fields.
			boolean found = false;
			while(!found)
			{
				String s = inbound.readLine();
				if(s != null)
				{
					inputHeader += "\n\t" + s;
					if(s.contains("If-Modified-Since"))
					{
						ifModSince = s.split(":", 2)[1];
						break;
					}

					if(s.length() == 0)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}

			// format a response from the server if a request header was received
			if(inputLine != null)
			{
				if (logging_enabled)
				{
					String s = "RECEIVED: " + inputHeader + "\n";

					writeToFile(s);
				}

				//tokenize the header for the request method and requested file.
				String tempStr = inputLine;
				StringTokenizer tok = new StringTokenizer(tempStr);
				method = tok.nextToken();
				fileName = tok.nextToken();
				
				// create the Date: field of the response header
				DateFormat df = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz");
				df.setTimeZone(TimeZone.getTimeZone("GMT"));
				Date date = new Date();
				dateString = "Date: " + df.format(date) + "\r\n";
				
				// if request method is supported, continue creating a response
				if(method.equals("GET"))
				{
					String file_n = "";
					lastModifiedString = "";
		
					// return the default response page
					if(fileName.equals("/"))
					{
						System.out.println("filename: " + fileName);
						file_n = docRoot + "\\index.html";
						File file = new File(file_n);
						if(!file.isFile())
						{
							statusCode = 404;
							statusString = "HTTP/1.1 404 Not Found" + "\r\n";
						}
						else
						{
							lastModifiedString = "Last-Modified: " + df.format(file.lastModified());
							lastModifiedString += "\r\n";
						}
					}
					// attempt to locate the file within docroot
					else
					{
						// favicon is alway requested, ignore this file request for the time being
						if(!fileName.equals("\favicon.ico"))
						{
							file_n = docRoot + "\\";
							file_n += fileName.substring(1);
							System.out.println("Requested: " + file_n);
							
							File file = new File(file_n);
							// if file does not exist or is not a file, return 404 Not Found
							if(!file.isFile())
							{
								statusCode = 404;
								statusString = "HTTP/1.1 404 Not Found" + "\r\n";
							}
							//file has been determined to exist
							else
							{
								//retrieve last modified data from file to be used in return header
								lastModifiedString = "Last-Modified: " + df.format(file.lastModified());
								lastModifiedString += "\r\n";

								// detemine if status code 304 is necessary
								if(ifModSince != null)
								{
									try
									{
										Date mod = df.parse(ifModSince.trim());
										Date last = df.parse(lastModifiedString.split(":",2)[1].trim());

										if(!last.after(mod))
										{
											statusCode = 304;
											statusString = "HTTP/1.1 304 Not Modified" + "\r\n";
										}
									}
									catch(ParseException e)
									{
										System.out.println(e);
									}
								}
							}
						}
					}

					// Parse the file extension to determine the content type
					contentTypeString = "Content-Type: ";
					if(file_n.endsWith(".htm") || file_n.endsWith(".html"))
					{
						contentTypeString += "text/html\r\n";
					}
					else if(file_n.endsWith(".txt"))
					{
						contentTypeString += "text/plain\r\n";
					}
					else if(file_n.endsWith(".jpg"))
					{
						contentTypeString += "image/jpeg\r\n";
					}
					else if(file_n.endsWith(".pdf"))
					{
						contentTypeString += "application/pdf\r\n";
					}
					else if(file_n.endsWith(".json"))
					{
						contentTypeString += "application/json\r\n";
					}				
					
					// file has been found, continue  creating header
					if(statusCode != 404)
					{
						// send response header
						FileInputStream file_in = new FileInputStream(file_n);
						lengthString = "Content-length: " + Integer.toString(file_in.available()) + "\r\n";
					
						outbound.writeBytes(statusString);
						outbound.writeBytes(serverString);
						outbound.writeBytes(contentTypeString);
						if(statusCode != 304)
						{
							outbound.writeBytes(lengthString);
						}
						outbound.writeBytes(dateString);
						outbound.writeBytes(lastModifiedString);
						outbound.writeBytes("\r\n");
					
						// if (logging) write respose to log file
						if (logging_enabled)
						{
							String s = "RESPONSE: \n\t" + statusString.trim() + "\n\t" +
							 	serverString.trim() + "\n\t" +
							  	contentTypeString.trim() + "\n\t" +
							   	lengthString.trim() + "\n\t" +
							    dateString.trim() + "\n\t" +
							    lastModifiedString.trim() + "\n\t";

							writeToFile(s);
						}

						//dont send data if page has not been modified.
						if(statusCode != 304)
						{
							byte[] buf = new byte[4096];
							int bytes_num = 0, total_sent = 0;
							while((bytes_num = file_in.read(buf)) != -1)
							{
								total_sent += bytes_num;
								outbound.write(buf, 0, bytes_num);
							}
							System.out.println("Sent " + total_sent + " Bytes");
						}
						file_in.close();
					}
					// file could not be found within docroot
					else
					{
						// Send back 404 response
						outbound.writeBytes(statusString);
						outbound.writeBytes(serverString);
						outbound.writeBytes("\r\n");
						
						String error = "<html><body<h1>404: Requested resource not found</h1></body></html>";
						outbound.writeBytes(error);

						// if (logging) write respose to log file
						if (logging_enabled)
						{
							String s = "RESPONSE: \n\t" + statusString.trim() + "\n\t" +
							 	serverString.trim() + "\n\t";

							writeToFile(s);
						}
					}
				}
				// request method was not GET
				else
				{		
					// return a 504 response
					statusString = "HTTP/1.1 501 Unimplemented" + "\r\n";
					
					outbound.writeBytes(statusString);
					outbound.writeBytes(serverString);
					outbound.writeBytes("\r\n");

					// if (logging) write respose to log file
					if (logging_enabled)
					{
						String s = "RESPONSE: \n\t" + statusString.trim() + "\n\t" +
						 	serverString.trim() + "\n\t";

						writeToFile(s);
					}
				}
				
				// clean up socket connection after processing
				System.out.println("Cleaning up connection: " + connectionSocket);
				outbound.close();
				inbound.close();
				connectionSocket.close();
			}
        
		}
		catch (IOException e)
		{
			System.out.println(e);
		}
	}
	
	/**
	 *	Write to the log file.
	 *
	 *	@param s The text to write to the log file.
	 *	@return void
	 */
	public void writeToFile(String s) 
	{
		try 
		{
			FileLock fl = null;
			FileOutputStream fos = new FileOutputStream(System.getProperty("user.dir") + "\\log\\" + log_file, true);
			FileChannel outChannel = fos.getChannel();
			while(fl == null)
				fl = outChannel.tryLock();

			ByteBuffer buf = ByteBuffer.allocate(1024);
			buf.put(s.getBytes());
			buf.flip();
		
			while(buf.hasRemaining())
				outChannel.write(buf);

			outChannel.force(false);
			fl.release();
		
			fos.close();

		} 
		catch (IOException e) {} 
		catch (OverlappingFileLockException e) {}
	}
}