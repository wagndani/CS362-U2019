
import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class ProjectB {

	@Rule
    public ErrorCollector collector = new ErrorCollector();
	
	@Before
	public void setUp() throws Exception {
	}

	@After
	public void tearDown() throws Exception {
			
	}
	
	@Test
	public void testIsValid() throws IOException {
		UrlValidator validator = new UrlValidator();
		
		try {
			File urls = new File("src/testUrls.txt");
			BufferedReader buffer = new BufferedReader(new FileReader(urls));
			String line = "";
			while((line = buffer.readLine()) != null) {
				String[] arr = line.split(":", 2);
				if(arr[0].equals("true")) {
					try {
						assertTrue(validator.isValid(arr[1]));
				    } catch (Throwable t) {
				        collector.addError(t);
				        System.out.print("\nError: " + arr[1] + " (Expected: True, Actual: False)\n");
				        continue;
				    }
				}
				else {
					try {
						assertFalse(validator.isValid(arr[1]));
				    } catch (Throwable t) {
				        collector.addError(t);
				        System.out.print("\nError: " + arr[1] + " (Expected: False, Actual: True)\n");
				        continue;
				    }
				}
			}
			buffer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
