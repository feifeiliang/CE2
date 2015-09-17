import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Collections;

/**
 * TextBuddy is a program that user can  store, retrieve and delete message.
 * User can perform these function by simply type a command line. 
 * The file can be saved to the disk when the user exit the program.
 * The command line format are below:
 * <pre>  
 * Welcome to TextBuddy. mytextfile.txt is ready for use
 * command: add little brown fox
 * added to mytextfile.txt: "little brown fox"
 * command:  display
 * 1. little brown fox
 * command: add jumped over the moon
 * added to mytextfile.txt: "jumped over the moon"
 * command: display
 * 1. little brown fox
 * 2. jumped over the moon
 * command: delete 2
 * deleted from mytextfile.txt: "jumped over the moon"
 * command: display
 * 1. little brown fox
 * command: clear
 * all content deleted from mytextfile.txt
 * command: display
 * mytextfile.txt is empty
 * command: exit
 * </pre>
 *
 * @author Liang Mengfei
 *
 */

public class TextBuddy {

	// error message
	// private static final String ERROR_EMPTY = "%s is empty";
	private static final String ERROR_BAD_ARGUMENT = "Bad argument \n %s";

	// message show
	private static final String MSG_WELCOME = "Welcome to TextBuddy. %s is ready for use";
	private static final String MSG_ADD = "added to %s: \"%s\"";
	private static final String MSG_CLEAR = "all content deleted from %s";
	private static final String MSG_DELETE = "deleted from %s: \"%s\"";
	private static final String MSG_EMPTY = "%s is empty";
	private static final String MSG_SEARCH_NOT_FOUND = "No results fonud in %s"; 


	// Commands
	private static final String COMMAND ="command: ";
	private static final String ADD_TEXT = "add";
	private static final String DISPLAY_TEXT = "display";
	private static final String DELETE_TEXT = "delete";
	private static final String CLEAR_TEXT = "clear";
	private static final String SEARCH_TEXT = "search";
	private static final String EXIT_PROGRAM = "exit";

	// store all texts from the user
	private static ArrayList <String> text; 
	private static ArrayList <String> searchText;

	// file contain texts are stored on disc
	private static File file;
	private static Scanner sc;

	// to store the command from user
	private static String command = "";



	public static void main(String[] args) throws IOException{
		checkArguments(args);
		setup(args[0]);   
		retrieveTexts(file);
		printWelcomeMessage();
		readCommandUntilExit();
	}

	// This method is to 
	// The user suppose to enter java program fileName
	private static void checkArguments(String[] args){
		if (args.length==0){
			messageDisplay(ERROR_BAD_ARGUMENT, "Proper Usuage is: Java program filename");
			System.exit(0);
		}
	}

	// this method is to setup the File and scanner that need to use 
	private static void setup (String fileName){
		file = new File(fileName);
		sc = new Scanner (System.in);
		text = new ArrayList <String> ();
	}

	// This method is to retrieve the last save text file from user
	// If the  
	private static void retrieveTexts (File file){
		try{
			Scanner scanner = new Scanner (file);
			while (scanner.hasNextLine()){
				text.add(scanner.nextLine());
			}
			scanner.close();
		}catch (FileNotFoundException e){
		}
	}

	private static void printWelcomeMessage(){
		messageDisplay(MSG_WELCOME,file.getName());
	}

	// This method is use to save the command that the user enter

	private static void readCommandUntilExit()throws IOException{

		command = readCommand();

		while (!command.equals(EXIT_PROGRAM)){

			switch(command){
				case ADD_TEXT : 
					addTexts();
					readCommand();
					continue;

				case DELETE_TEXT : 
					deleteTexts();
					readCommand();
					continue;

				case DISPLAY_TEXT : 
					displayTexts();
					readCommand();
					continue;

				case CLEAR_TEXT : 
					clearTexts();
					readCommand();
					continue;
					
				case SEARCH_TEXT :
				     search();
				     displaySearchResult();
				     readCommand(); 
					 break;

				case EXIT_PROGRAM : 
					break;

				default: 
					// if the user any command that cannot be identified (e.g "jump")
					messageDisplay(ERROR_BAD_ARGUMENT, "Please enter a valid command");
					readCommand();
					continue;
			}
		}   
		saveToFile(); 
	}

	/**
	 * This operation is used to read command from the user.
	 * 
	 * @return the command
	 */
	private static String readCommand (){

		System.out.print(COMMAND);
		command = sc.next().toLowerCase();
		return command;

	}

	// This operation is used to add the text.
	private static void addTexts(){
		String message = sc.nextLine().trim();
		text.add(message);
		messageDisplay(MSG_ADD, file.getName(), message);
		sortText();

	}

	// This method is used to sort the arraylist alphabetically
	private static void sortText(){
		Collections.sort(text);
	}

	// Delete command performed
	private static void deleteTexts() throws IOException{
		try{

			int index = sc.nextInt();
			index = index-1;
			if (index > 0 && index>=text.size()){
				messageDisplay(ERROR_BAD_ARGUMENT, "index out of range");
				sc.nextLine();
				//readCommandUntilExit();
				return;
			}
			String message = text.get(index);
			System.out.println(message);
			if (text.size() == 0) {

				messageDisplay(MSG_EMPTY, file.getName());
			} else {
				text.remove(index);
				messageDisplay(MSG_DELETE, file.getName(), message);
			}
		} catch (java.util.InputMismatchException e){
			messageDisplay(ERROR_BAD_ARGUMENT, " Please enter a valid index");
			sc.nextLine();
			//readCommandUntilExit(); 
			return;
		} catch (java.lang.ArrayIndexOutOfBoundsException e){

			messageDisplay(ERROR_BAD_ARGUMENT, " Please enter a valid index");
			sc.nextLine();
			//readCommandUntilExit(); 
			return;
		}
	}

	// Display command performed
	private static void displayTexts(){
		sc.nextLine();
		int size = text.size();
		if (size == 0) {
			messageDisplay(MSG_EMPTY, file.getName());
		} else {
			int num=1;
			String label;
			for (int i = 0; i < size; i++){
				label = String.valueOf(num++)+".";
				System.out.println(label + " " + text.get(i));
			}
		}

	}

	// Clear command performed
	private static void clearTexts(){
		sc.nextLine();
		if (text.size() == 0) {
			messageDisplay(MSG_EMPTY, file.getName()); 
		} else {
			text.clear();
			messageDisplay(MSG_CLEAR, file.getName());
		}
	}

	// Search for a word in the file
	private static void search(){
		String word = sc.nextLine().trim();
		searchText = new ArrayList <String> ();
		int size = text.size();
		if (size == 0){
			messageDisplay(MSG_EMPTY, file.getName());
		} else {
			int i; 
			for (i = 0; i < size; i++){
				if (text.get(i).contains(word)){
					searchText.add(text.get(i));
				}
			}
		}
	}

	// Display the search result 
	private static void displaySearchResult (){
	
		int size = searchText.size();
		if (size == 0) {
			messageDisplay(MSG_SEARCH_NOT_FOUND, file.getName());
		} else {
			int num=1;
			String label;
			for (int i = 0; i < size; i++){
				label = String.valueOf(num++)+".";
				System.out.println(label + " " + searchText.get(i));
			}
		}
	}

	// This method is to store all the text int he arraylist into the text file
	private static void saveToFile() throws FileNotFoundException{
		try {
			FileWriter fileW = new FileWriter (file);
			BufferedWriter buffW = new BufferedWriter (fileW);
			for (int i = 0; i < text.size(); i++){
				buffW.write(text.get(i));
				buffW.newLine();
			}
			buffW.close();	
		} catch (IOException e){
		}

	}


	//This method is used to print out the error message
	//pre: errorType!=null && message!=null
	private static void messageDisplay(String message, Object... arguments){
		System.out.println(String.format(message,arguments));
	}
}
