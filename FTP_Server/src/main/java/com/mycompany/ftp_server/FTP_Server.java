/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.ftp_server;
import java.io.*;
import java.net.*;
import java.util.Enumeration;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

/**
 *
 * @author Kulindu Kodithuwakku
 */
public class FTP_Server {

    public static void main(String[] args) {
       int port = 6969;
       
       try(ServerSocket serverSocket = new ServerSocket(port)){
           
            printServerIPs();
           
           System.out.println("Server listening on port:"+ port);
           
           Socket clientSocket = serverSocket.accept();
           System.out.println("connection establixhed with "+ clientSocket.getInetAddress());
           
           Scanner scanner = new Scanner(System.in);
           System.out.println("Enter file path");
           String filePath = scanner.nextLine();
           File fileToSend = new File(filePath);
           
           if(!fileToSend.exists()){
               System.out.println("File not found: "+ filePath );
               return;
           }
           
           try(BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(fileToSend));
               BufferedOutputStream outputStream = new BufferedOutputStream(clientSocket.getOutputStream())){
           
               byte[] buffer  = new byte[1024];
               int bytesRead;
               
               while((bytesRead = fileInputStream.read(buffer)) != -1){
                   outputStream.write(buffer,0,bytesRead);
               }
               System.out.println("File Sent Successfully");
           }catch(IOException e){
                e.printStackTrace();
           }
       }catch (IOException e){
           e.printStackTrace();
       }
    }
    private static void printServerIPs() throws SocketException{
        Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
        
        while(networkInterfaces.hasMoreElements()){
            NetworkInterface networkInterface =  networkInterfaces.nextElement();
            
            System.out.println("Interface: "+ networkInterface.getDisplayName());
            
            Enumeration<InetAddress> inetAddresses = networkInterface.getInetAddresses();
            while(inetAddresses.hasMoreElements()){
                InetAddress address = inetAddresses.nextElement();
                System.out.println("IP: "+ address.getHostAddress());
            }
            System.out.println("---------------------");
        }
    }
}

