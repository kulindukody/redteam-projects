/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.caesarciphertool;
import java.util.Scanner;
/**
 *
 * @author Kulindu Kodithuwakku
 */
public class CaesarCipherTool {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("Enter Message: ");
        String message = scanner.nextLine();
        
        System.out.println("Enter value to shift");
        int shift = scanner.nextInt();
        
        System.out.println("Encrypt(E) or Decrypt(D)");
        char choice = scanner.next().charAt(0);
      
        
        String Result;
        if (Character.toUpperCase(choice)== 'E'){
            Result  = encrypt(message,shift);
            System.out.print("Encrypted Message: " + Result);
        }else if(Character.toUpperCase(choice)=='D'){
            Result = decrypt(message,shift);
            System.out.print("Decrypted Message: " + Result);
        }else{
            System.out.println("Invalid Choice. Please Enter 'E' for encrypt and 'D' for decryption.");
        }        
    }
    
    private static String encrypt(String message,int shift){
        StringBuilder encryptedMessage = new StringBuilder();
        
        for(char ch : message.toCharArray()){
            if(Character.isLetter(ch)){
                char base = Character.isUpperCase(ch)? 'A':'a';
                char encryptedChar = (char) ((ch - base + shift + 26)% 26 + base);
                encryptedMessage.append(encryptedChar);
            }else{
                encryptedMessage.append(ch);
            }
        }
        return encryptedMessage.toString();
    }
    private static String decrypt(String message,int shift){
        return encrypt(message,-shift);
    }
}
