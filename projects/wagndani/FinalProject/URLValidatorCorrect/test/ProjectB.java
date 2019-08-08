import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class ProjectB {

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
					assertTrue(validator.isValid(arr[1]));
				}
				else {
					assertFalse(validator.isValid(arr[1]));
				}
			}
			buffer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
