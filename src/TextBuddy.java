import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Collections;
import java.nio.file.Path;
import java.nio.file.Paths;

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
	public static final String ERROR_BAD_ARGUMENT = "Bad argument: %s \n";

	// message show
	public static final String MSG_WELCOME = "Welcome to TextBuddy. %s is ready for use \n";
	public static final String MSG_ADD = "added to %s: \"%s\" \n";
	public static final String MSG_CLEAR = "All contents are removed! \n";
	public static final String MSG_DELETE = "deleted from %s: \"%s\" \n";
	public static final String MSG_EMPTY = "%s is empty \n";
	public static final String MSG_SEARCH_NOT_FOUND = "No results fonud in %s \n"; 
	private static final String MSG_TASKS_SORTED = "All tasks have been sorted alphabetically. \n";


	// Commands
	public static final String COMMAND ="command: ";
	public static final String ADD_TEXT = "add";
	public static final String DISPLAY_TEXT = "display";
	public static final String DELETE_TEXT = "delete";
	public static final String CLEAR_TEXT = "clear";
	public static final String SEARCH_TEXT = "search";
	public static final String SORT_TEXT = "sort";
	public static final String EXIT_PROGRAM = "exit";

	// store all texts from the user
	public static ArrayList <String> text; 

	// file contain texts are stored on disc
	public static File file;
	public static Scanner sc;
	public static Path path;

	// to store the command from user
	public static String command = "";
	
	
	public TextBuddy() {
		path = Paths.get("textout.txt");
		text = new ArrayList<String>();
	}
	
	public static void main(String[] args) throws IOException{
		checkArguments(args);
		setup(args[0]);   
		retrieveTexts(file);
		printWelcomeMessage();
		readCommandUntilExit();
	}

	// This method is to 
	// The user suppose to enter java program fileName
	public static void checkArguments(String[] args){
		if (args.length==0){
			displayResult (String.format(ERROR_BAD_ARGUMENT, "Proper Usuage is: Java program filename"));
			System.exit(0);
		}
	}

	// this method is to setup the File and scanner that need to use 
	public static void setup (String fileName){
		file = new File("src/textout.txt");
		sc = new Scanner (System.in);
		text = new ArrayList <String> ();
		path = Paths.get("textout.txt");
	}

	// This method is to retrieve the last save text file from user
	// If the  
	public static void retrieveTexts (File file){
		try{
			Scanner scanner = new Scanner (file);
			while (scanner.hasNextLine()){
				text.add(scanner.nextLine());
			}
			scanner.close();
		}catch (FileNotFoundException e){
		}
	}

	public static void printWelcomeMessage(){
		displayResult(String.format(MSG_WELCOME,path.toString()));
	}

	// This method is use to save the command that the user enter

	public static void readCommandUntilExit()throws IOException{
		while (true) {
			System.out.print(COMMAND);
			String input = sc.nextLine();
			String output = performCommand(input);
			if (output.equals(EXIT_PROGRAM)){
				break;
			}
			displayResult(output);
		}
		saveToFile(); 
	}
	
	public static String performCommand(String input) throws IOException{
		String displayMessage = null;
	    command = getFirstWord(input);
			switch(command){
				case ADD_TEXT : 
					displayMessage = addTexts(removeFirstWord(input));
					break;

				case DELETE_TEXT : 
					displayMessage = deleteTexts(removeFirstWord(input));
					break;

				case DISPLAY_TEXT : 
					displayMessage = displayTexts();
					break;

				case CLEAR_TEXT : 
					displayMessage = clearTexts();
					break;
					
				case SEARCH_TEXT :
				     displayMessage = search(removeFirstWord(input));
					 break;
				
				case SORT_TEXT :
				     displayMessage = sortText();
					 break;
				case EXIT_PROGRAM :
					displayMessage = EXIT_PROGRAM;
					break;
					 
				default: 
					// if the user any command that cannot be identified (e.g "jump")
					displayMessage = String.format(ERROR_BAD_ARGUMENT, "Please enter a valid command");
					break;
			}
			return displayMessage;
	}

	public static String removeFirstWord(String userCommand) {
		return userCommand.replace(getFirstWord(userCommand), "").trim();
	}

	public static String getFirstWord(String userCommand) {
		String commandTypeString = userCommand.trim().split("\\s+")[0].toLowerCase();
		return commandTypeString;
	}

	// This operation is used to add the text.
	public static String addTexts(String message){
		text.add(message.trim());
		return String.format(MSG_ADD, path.toString(),message);
	}

	// This method is used to sort the arraylist alphabetically
	public static String sortText(){
		Collections.sort(text);
		return MSG_TASKS_SORTED + displayTexts();
	}

	// Delete command performed
	public static String deleteTexts(String message) throws IOException{
		try{
			int index = Integer.parseInt(message);
			index = index-1;
			if (index > 0 && index>=text.size()){
				return String.format(ERROR_BAD_ARGUMENT, "index out of range");
			}
			if (text.size() == 0) {

				return String.format(MSG_EMPTY, path.toString());
			} else {
				message = text.get(index);
				text.remove(index);
				return String.format(MSG_DELETE, path.toString(),message );
			}
		} catch (java.lang.NumberFormatException e){
			return String.format(ERROR_BAD_ARGUMENT, " Please enter a valid index");
		} catch (java.lang.ArrayIndexOutOfBoundsException e){
			return String.format(ERROR_BAD_ARGUMENT, " Please enter a valid index");
		}
	}

	// Display command performed
	private static String displayTexts(){
		int size = text.size();
		String result = "";
		if (size == 0) {
			result = String.format(MSG_EMPTY,path.toString());
		} else {
			int num = 1;
			String label;
			for (int i = 0; i < size; i++){
				label = String.valueOf(num++) + ".";
				result += label + " " + text.get(i) + "\n";
			}
		}
		return result;
	}

	// Clear command performed
		private static String clearTexts(){
			if (text.size() == 0) {
				return String.format(MSG_EMPTY, path.toString()); 
			} else {
				text.clear();
				return String.format(MSG_CLEAR);
			}
		}

	// Search for a word in the file
	public static String search(String word){
		String result = ""; 
		int size = text.size();
		if (size == 0){
			result = "Result not found! \n";
		} else { 
			for (int i = 0; i < size; i++){
				if (text.get(i).contains(word)){
					result += (i+1) + ". "+text.get(i) + "\n";
				}
			}
		}
		return result;
	}

	//This method is used to print out the message
	//pre: message!=null
	public static void displayResult (String message){
		System.out.print(message);
	}

	// This method is to store all the text int he arraylist into the text file
	public static void saveToFile() throws FileNotFoundException{
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

	/**
	 * @return the string form of the file passed by the user.
	 */
	static String showPath(){
		return path.toString();
	}
}
