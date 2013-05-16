import robocode.control.*;
import robocode.control.events.*;
import java.io.*;
import java.net.*;

public class RobocodeRunner {

	public static void main(String[] args) throws IOException {
		Socket clientSocket = new Socket(InetAddress.getLocalHost(),9000);
		
		
		
		RobocodeEngine engine = new RobocodeEngine(new java.io.File("/Development/robocode"));
		//RobocodeEngine engine = new RobocodeEngine();
		System.out.println(engine.getRobotsDir());
		//RobotSpecification[] allRobots = engine.getLocalRepository();
		//for (RobotSpecification result: allRobots) {
		//	System.out.println(result.getName());
		//}
		
		engine.setVisible(false);
		int numberOfRounds = 15;
		BattlefieldSpecification battlefield = new BattlefieldSpecification(800,600);
		
		
		PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
		BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		engine.addBattleListener(new BattleObserver(out));

		String inputLine,outputLine;
		out.println("Next");
		System.out.println("Sending Next...");
		while ((inputLine = in.readLine()) != null) {
			System.out.println(inputLine);
			if (inputLine.equals("bye")) break;
		if (inputLine.equals("ack") || inputLine.equals("ack\n")) { System.out.println("Ack received");
		} else {
			System.out.println("Setting up the battle");	
//		RobotSpecification[] selectedRobots = engine.getLocalRepository("sample.MyFirstRobot,sample.SpinBot");
			RobotSpecification[] selectedRobots = engine.getLocalRepository(inputLine);
		
			BattleSpecification battleSpec = new BattleSpecification(numberOfRounds, battlefield, selectedRobots);
			engine.runBattle(battleSpec,true);
			System.out.println("Done");
		//	out.println("Next");
			
			}
		}
		out.close();
		in.close();
		clientSocket.close();
		// engine.runBattle(battleSpec,true);
		
		engine.close();

		System.exit(0);
      }
	public void onBattleCompleted(BattleCompletedEvent e) {
		System.out.println("Battle Finished");
		System.out.println("Results:");
		for (robocode.BattleResults result : e.getSortedResults()) {
			System.out.println(" " + result.getTeamLeaderName() + ": " + result.getScore());
		}
	}

	public void onBattleMessage(BattleMessageEvent e) {
		System.out.println("Msg> " + e.getMessage());
	}

	public void onBattleError(BattleErrorEvent e) {
		System.out.println("Err> " + e.getError());
	}


}

class BattleObserver extends BattleAdaptor {
	PrintWriter out;
	
	BattleObserver(PrintWriter theWriter) {
		out = theWriter;
	}
	
	public void onBattleCompleted(BattleCompletedEvent e) {
		System.out.println("Battle Finished");
		System.out.println("Results:");
		for (robocode.BattleResults result : e.getIndexedResults()) {
			int i = result.getScore();
			if (i==0) i=1; // min score of 1.
			out.print("" + i  + ", ");
			System.out.println("" + i);
		}
		out.println("");
	}
/*
	public void onBattleMessage(BattleMessageEvent e) {
		System.out.println("Msg> " + e.getMessage());
	}
*/
	public void onBattleError(BattleErrorEvent e) {
		System.out.println("Err> " + e.getError());
	}
}

