import static org.junit.Assert.*;
import java.io.IOException;
//import org.junit.BeforeClass;
import org.junit.Test;
import java.util.ArrayList;

public class TextBuddyTest {
	
	private static final String MSG_INVALID_TYPE = "Bad argument: Please enter a valid command \n";
	private static final String MSG_DELETE_TASK_ERROR = "Bad argument: index out of range \n";
	private static final String MSG_DELETED_TASK = "deleted from %1$s: \"%2$s\" \n";
	private static final String MSG_ADDED_TASK = "added to %1$s: \"%2$s\" \n";
	private static final String MSG_TASKS_CLEARED = "All contents are removed! \n";
	
	static ArrayList<String> expectList;
	
	public TextBuddyTest() {
		expectList = new ArrayList<String>();
	}
	
	public static void populateList(int numberOfItems){
		for (int i = 0; i < numberOfItems; i++) {
			expectList.add(String.valueOf(i+1));
		}
	}
	
	@Test
	public void testSimpleAdd() throws IOException {
		TextBuddy textbuddy = new TextBuddy();
		expectList.clear();
		populateList(1);
		assertEquals(String.format(MSG_ADDED_TASK, textbuddy.showPath(), "1"), textbuddy.performCommand("add 1"));
		assertEquals(expectList, TextBuddy.text);
	}
	
	@Test
	public void testSimpleDelete() throws IOException {
		TextBuddy textbuddy = new TextBuddy();
		expectList.clear();
		populateList(3);
		expectList.remove(1);
		textbuddy.performCommand("add 1");
		textbuddy.performCommand("add 2");
		textbuddy.performCommand("add 3");
		assertEquals(String.format(MSG_DELETED_TASK, textbuddy.showPath(), "2"), textbuddy.performCommand("delete 2"));
		assertEquals(expectList, textbuddy.text);
	}
	
	@Test
	public void testDeleteTaskThatDoesNotExist() throws IOException {
		TextBuddy textbuddy = new TextBuddy();
		expectList.clear();
		populateList(3);
		textbuddy.performCommand("add 1");
		textbuddy.performCommand("add 2");
		textbuddy.performCommand("add 3");
		assertEquals(MSG_DELETE_TASK_ERROR, textbuddy.performCommand("delete 4"));
		assertEquals(expectList, textbuddy.text);
	}
	
	@Test
	public void testClear() throws IOException {
		TextBuddy textbuddy = new TextBuddy();
		expectList.clear();
		textbuddy.performCommand("add 1");
		textbuddy.performCommand("add 2");
		textbuddy.performCommand("add 3");
		assertEquals(MSG_TASKS_CLEARED, textbuddy.performCommand("clear"));
		assertEquals(expectList, textbuddy.text);
	}
	
	@Test
	public void testInvalidType() throws IOException {
		TextBuddy textbuddy = new TextBuddy ();
		assertEquals(MSG_INVALID_TYPE, textbuddy.performCommand("asdas"));
	}


}
