import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.Random;

import static org.junit.Assert.assertEquals;

public class randomTest {

	@Before
	public void setUp() throws Exception {
		
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testIsValid() {
		UrlValidator validator = new UrlValidator();
		
		int n = 150;
		int i, j, k;
		boolean valid = true; 
		Random rand = new Random();
		StringBuilder urlBuilder = new StringBuilder();
		String url = "";
		for(i = 0; i < n; i++) {
			for(j = 0; j < 5; j++) {
				k = rand.nextInt(testData[j].length);
				urlBuilder.append(testData[j][k].item);
				valid = valid && testData[j][k].valid;
			}
			url = urlBuilder.toString();
			urlBuilder.setLength(0);
			assertEquals(validator.isValid(url), valid);
			valid = true;
		}	
	}

	// Test Data
	

	 ResultPair[] testUrlScheme = {new ResultPair("http://", true),
             new ResultPair("ftp://", true),
             new ResultPair("h3t://", false),
             new ResultPair("3ht://", false),
             new ResultPair("http:/", false),
             new ResultPair("http:", false),
             new ResultPair("http/", false),
             new ResultPair("://", false)};

	 ResultPair[] testUrlAuthority = {new ResultPair("www.google.com", true),
                new ResultPair("www.google.com.", true),
                new ResultPair("go.com", true),
                new ResultPair("go.au", true),
                new ResultPair("0.0.0.0", true),
                new ResultPair("255.255.255.255", true),
                new ResultPair("256.256.256.256", false),
                new ResultPair("255.com", true),
                new ResultPair("1.2.3.4.5", false),
                new ResultPair("1.2.3.4.", false),
                new ResultPair("1.2.3", false),
                new ResultPair(".1.2.3.4", false),
                new ResultPair("go.a", false),
                new ResultPair("go.a1a", false),
                new ResultPair("go.cc", true),
                new ResultPair("go.1aa", false),
                new ResultPair("aaa.", false),
                new ResultPair(".aaa", false),
                new ResultPair("aaa", false),
                new ResultPair("", false)};
	 
	 ResultPair[] testUrlPort = {new ResultPair(":80", true),
             new ResultPair(":65535", true), // max possible
             new ResultPair(":65536", false), // max possible +1
             new ResultPair(":0", true),
             new ResultPair("", true),
             new ResultPair(":-1", false),
             new ResultPair(":65636", false),
             new ResultPair(":999999999999999999", false),
             new ResultPair(":65a", false)};
	 
	 ResultPair[] testPath = {new ResultPair("/test1", true),
          new ResultPair("/t123", true),
          new ResultPair("/$23", true),
          new ResultPair("/..", false),
          new ResultPair("/../", false),
          new ResultPair("/test1/", true),
          new ResultPair("", true),
          new ResultPair("/test1/file", true),
          new ResultPair("/..//file", false),
          new ResultPair("/test1//file", false)};
	 
	 ResultPair[] testUrlQuery = {new ResultPair("?action=view", true),
             new ResultPair("?action=edit&mode=up", true),
             new ResultPair("", true)};
	 
	 ResultPair[][] testData = { testUrlScheme, testUrlAuthority, testUrlPort, testPath, testUrlQuery };

}
