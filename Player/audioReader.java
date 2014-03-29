
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.Timer;
import java.util.Scanner;
import java.util.TimerTask;
import java.util.Arrays;
import javax.sound.sampled.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class audioReader {

   public static Timer timer;
   public static Task t;
   public static JFrame f;
   public static metaDataFile mdf;
   public static int count = 0;
   public static int addptr = -1;
   public static int moveptr = 0;
   public static int duration = 0;
   public static int curFrame = 0;
   public static int prevFlag = 0;
   public static int nextFlag = 0;
   public static int syncFlag = 0;
   public static String audio = null;
   public static Clip clip;
   public static MyAudioThread aThread;
   public static BufferedImage[] img;
   public static String[] prevNextBuffer;
   public static int tlx, tly, brx, bry;
   public static JButton button1, button2, button3, button4, button5;
   
   public static void main(String[] args) throws IOException {   	

	String fileName = args[0];
	mdf = new metaDataFile(args[1]);
	prevNextBuffer = new String[25];
	prevNextBuffer[++addptr] = fileName;
	moveptr = addptr;
	videoLoader(fileName);
	audio = args[2];
	aThread = new MyAudioThread(audio,0);
	videoPlayer(img);
	aThread.start();
   }
	
   public static void videoLoader (String filename) {	
	
	int width = 352;
	int height = 288;
	int ind = 0;
	int z = 0;
	
	try {
	    File file = new File(filename);
	    InputStream is = new FileInputStream(file);

	    long len = file.length();
	    byte[] bytes = new byte[(int)len];
	    int numFrames = (int)len/(width*height*3);
		img = new BufferedImage[numFrames]; 
		audio = filename.substring(0, filename.length() - 3) + "pcm";
		
		int offset = 0;
        int numRead = 0;
        while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
            offset += numRead;
        }
    		
    	for(int i = 0; i < numFrames; i++){
		  
		  img[i] = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		  ind = width * height * 3 * i;
		  
		  for(int y = 0; y < height; y++){
			  for(int x = 0; x < width; x++){
				  
				  byte a = 0;
				  byte r = bytes[ind];
				  byte g = bytes[ind+height*width];
				  byte b = bytes[ind+height*width*2]; 
				
				  int pix = 0xff000000 | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
				  //int pix = ((a << 24) + (r << 16) + (g << 8) + b);
				  img[i].setRGB(x,y,pix);
				  ind++;
			  }
		  }
		
		  Graphics2D g = img[i].createGraphics();
		  for(z = 0; z < count; z++) {
		     if(i > mdf.keyFrame[z][0] && i < mdf.keyFrame[z][mdf.numKeyFrames[z]-1]) {
		         interpolate(i,z);
				 g.drawRect(tlx,tly,brx-tlx,bry-tly);
		     }
		  }
		}
	
	} catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }
    

   }
   
   public static void videoPlayer (BufferedImage img[]) {
   
    int frameRate = 24;
	duration = 1000/frameRate;
	timer = new Timer();
	f = new JFrame();
	f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	JPanel panel = new JPanel();
    ImageIcon play = new ImageIcon("play.png");
	ImageIcon stop = new ImageIcon("stop.png");
	ImageIcon pause = new ImageIcon("pause.png");
	ImageIcon next = new ImageIcon("next.png");
	ImageIcon prev = new ImageIcon("prev.png");
	play.setImage(play.getImage().getScaledInstance(20,20,Image.SCALE_SMOOTH));
	stop.setImage(stop.getImage().getScaledInstance(20,20,Image.SCALE_SMOOTH));
	pause.setImage(pause.getImage().getScaledInstance(20,20,Image.SCALE_SMOOTH));
	next.setImage(next.getImage().getScaledInstance(20,20,Image.SCALE_SMOOTH));
	prev.setImage(prev.getImage().getScaledInstance(20,20,Image.SCALE_SMOOTH));
	button1 = new JButton(prev);
	button1.setEnabled(false);
    button2 = new JButton(next);
	button2.setEnabled(false);
	button3 = new JButton(play);
	button3.setEnabled(false);
	button4 = new JButton(pause);
	button5 = new JButton(stop);
	panel.add(button1);
	panel.add(button2);
	panel.add(button3);
	panel.add(button4);
	panel.add(button5);
	button1.addActionListener(new ActionPrev());
	button2.addActionListener(new ActionNext());
	button3.addActionListener(new ActionPlay());
	button4.addActionListener(new ActionPause());
	button5.addActionListener(new ActionStop());
	f.getContentPane().add(panel, BorderLayout.SOUTH);
	t = new Task(img,f,curFrame);
	syncFlag = 1;
	timer.schedule( t, 0, duration);
   }
   
   public static void interpolate (int frameNum, int link) {
     
	 int a = 0, b = 0;
     int totalFrames = 0;
     int keyIndex = 1;
     tlx = 0; tly = 0; brx = 0; bry = 0;
	
     do {
	     if(mdf.keyFrame[link][keyIndex-1] < frameNum && mdf.keyFrame[link][keyIndex] > frameNum) {
		     a = frameNum - mdf.keyFrame[link][keyIndex-1];
		     b = mdf.keyFrame[link][keyIndex] - frameNum;
		     totalFrames = mdf.keyFrame[link][keyIndex] - mdf.keyFrame[link][keyIndex-1];
				
		     //interpolated values of topLeft and bottomRight coordinates 
		     tlx = mdf.topLeftX[link][keyIndex-1] * b / totalFrames + mdf.topLeftX[link][keyIndex] * a / totalFrames;
		     tly = mdf.topLeftY[link][keyIndex-1] * b / totalFrames + mdf.topLeftY[link][keyIndex] * a / totalFrames;
		     brx = mdf.bottomRightX[link][keyIndex-1] * b / totalFrames + mdf.bottomRightX[link][keyIndex] * a / totalFrames;
		     bry = mdf.bottomRightY[link][keyIndex-1] * b / totalFrames + mdf.bottomRightY[link][keyIndex] * a / totalFrames;
	     }
	  
	     keyIndex++;
  
     } while(keyIndex < mdf.numKeyFrames[link]);

   }
   
   static class ActionPrev implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Prev!");
		 button2.setEnabled(true);
		 button3.setEnabled(false);
		 prevFlag = 1;
		 curFrame = 0;
		 timer.cancel();
         timer.purge();
		 clip.stop();
		 f.getContentPane().remove(1);
		 MyThread pThread = new MyThread(prevNextBuffer[--moveptr]);
         pThread.start();
		 
		 if(moveptr == 0)
		     button1.setEnabled(false);
			 
		 try {
             Thread.sleep(1000);
         } catch(InterruptedException ex) {
             Thread.currentThread().interrupt();
         }
		 
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 syncFlag = 1;
		 timer.schedule(t, 0, duration);
		 aThread = new MyAudioThread(audio, curFrame);
		 aThread.start();
     }
   }
   
   static class ActionNext implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Next!");
		 button1.setEnabled(true);
		 button3.setEnabled(false);
		 nextFlag = 1;
         curFrame = 0;	
         timer.cancel();
         timer.purge();
		 clip.stop();
		 f.getContentPane().remove(1);
		 MyThread nThread = new MyThread(prevNextBuffer[++moveptr]);
         nThread.start();
		 
		 if(moveptr == addptr) 
		     button2.setEnabled(false);
		 
		 try {
             Thread.sleep(1000);
         } catch(InterruptedException ex) {
             Thread.currentThread().interrupt();
         }
		 
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 syncFlag = 1;
		 timer.schedule(t, 0, duration);
		 aThread = new MyAudioThread(audio, curFrame);
		 aThread.start();
     }
   }
   
   static class ActionPlay implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Play!"); 		 
		 button3.setEnabled(false);
		 f.getContentPane().remove(1);
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 syncFlag = 1;
		 timer.schedule(t, 0, duration);
         aThread = new MyAudioThread(audio, curFrame);
         aThread.start();		 
     }
   }
   
   static class ActionPause implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Pause!");
         button3.setEnabled(true);
		 curFrame = t.c;
		 timer.cancel();
         timer.purge();		
         clip.stop();		 
     }
   }
   
   static class ActionStop implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Stop!");
         button3.setEnabled(true);
		 curFrame = 0;
		 timer.cancel();
         timer.purge();
         clip.stop();		 
     }
   }
  
}

class metaDataFile {
     public static int[] id = new int[10];
	 public static String[] name = new String[10];
	 public static int[] numKeyFrames = new int[10];
	 public static String[] linkedVideo = new String[10];
	 public static int[] linkedVideoFrame = new int[10];
	 public static int[][] keyFrame = new int[10][10];
	 public static int[][] topLeftX = new int[10][10];
	 public static int[][] topLeftY = new int[10][10];
	 public static int[][] bottomRightX = new int[10][10];
	 public static int[][] bottomRightY = new int[10][10];
	 
	 public metaDataFile() { }
	 public metaDataFile(String file) throws IOException { 
	     //initialize the arrays to 0
	     int i = 0;
		 audioReader.count = 0;
		 Arrays.fill(id, 0);
		 Arrays.fill(name, null);
		 Arrays.fill(numKeyFrames, 0);
		 Arrays.fill(linkedVideo, null);
		 Arrays.fill(linkedVideoFrame, 0);
		 for(i = 0; i < 10; i++) {
             Arrays.fill(keyFrame[i], 0);
			 Arrays.fill(topLeftX[i], 0);
			 Arrays.fill(topLeftY[i], 0);
			 Arrays.fill(bottomRightX[i], 0);
			 Arrays.fill(bottomRightY[i], 0);
         }		 
		 
		 //read from meta file
		 Scanner s = null;
	     try {        
	         s = new Scanner(new BufferedReader(new FileReader(file)));
             while (s.hasNextInt()) {
				 id[audioReader.count] = s.nextInt();
				 name[audioReader.count] = s.next();
				 numKeyFrames[audioReader.count] = s.nextInt();
				 linkedVideo[audioReader.count] = s.next();
				 linkedVideoFrame[audioReader.count] = s.nextInt();
				 
				 for(i = 0; i < numKeyFrames[audioReader.count]; i++) {
				     keyFrame[audioReader.count][i] = s.nextInt();
					 topLeftX[audioReader.count][i] = s.nextInt();
					 topLeftY[audioReader.count][i] = s.nextInt();
					 bottomRightX[audioReader.count][i] = s.nextInt();
					 bottomRightY[audioReader.count][i] = s.nextInt();
				 }
				 
                 audioReader.count++;
			 }
         } finally {
             if (s != null) {
                 s.close();
             }
         }
	 
	 }

}

class Task extends TimerTask implements MouseListener {
   int c = 0;
   int u = 0;
   int flag = 0;
   boolean falseAlarm = true;
   int locationX = 0, locationY = 0;
   BufferedImage myImg[] = new BufferedImage[100];
   JFrame frame = new JFrame();
   JLabel label = new JLabel();
  
   public Task() { }
   public Task(BufferedImage r[], JFrame f, int cFrame) {
	  myImg = r;
	  frame = f;
	  c = cFrame;
   }
   
   public void run() {
	  if(c == myImg.length) 
	    c = 0;
			 
	  ImageIcon icon = new ImageIcon(myImg[c++]);
      icon.getImage().flush();
      label.setIcon( icon );
	  label.addMouseListener(this);
	  frame.getContentPane().add(label, BorderLayout.NORTH);
	  frame.pack();
	  frame.setVisible(true);
	  
	  if(flag == 1) {
		 this.cancel();
		 audioReader.clip.stop();
		 flag = 0;
		 frame.remove(label);
		 
		 //check if clicked location is within bounding box of any hyperlink
		 for(u = 0; u < audioReader.count; u++) { 
		     if(c > audioReader.mdf.keyFrame[u][0] && c < audioReader.mdf.keyFrame[u][audioReader.mdf.numKeyFrames[u]-1]) {
		         audioReader.interpolate(c,u);
		         if(locationX >= audioReader.tlx && locationX <= audioReader.brx && locationY >= audioReader.tly && locationY <= audioReader.bry) {
                     falseAlarm = false;
                     break;
                 }				 
		     }
		 }
		 
		 if(falseAlarm == false) {
		     falseAlarm = true;
			 int startFrame = audioReader.mdf.linkedVideoFrame[u];
			 
			 int sleepTime = 0;
			 if(startFrame < 180)
			     sleepTime = 1200;
			 else if(startFrame > 180 && startFrame < 360)
			     sleepTime = 2000;
			 else if(startFrame > 360 && startFrame < 540)
			     sleepTime = 3000;
			 else
			     sleepTime = 4000;
			 
			 MyThread myThread = new MyThread(audioReader.mdf.linkedVideo[u]);
             myThread.start();
		 
		     try {
                 Thread.sleep(sleepTime);
             } catch(InterruptedException ex) {
                 Thread.currentThread().interrupt();
             }
		 
			 audioReader.timer = new Timer();
		     audioReader.t = new Task(audioReader.img,frame,startFrame);
		     audioReader.syncFlag = 1;
			 audioReader.timer.schedule(audioReader.t, 0, 1000/24);
			 audioReader.aThread = new MyAudioThread(audioReader.audio, startFrame);
             audioReader.aThread.start();
	  
	     } else {
		     audioReader.timer = new Timer();
		     audioReader.t = new Task(audioReader.img,frame,c);
		     audioReader.syncFlag = 1;
			 audioReader.timer.schedule(audioReader.t, 0, 1000/24);
			 audioReader.aThread = new MyAudioThread(audioReader.audio, c);
             audioReader.aThread.start();
		 }
	  }
	 
   }

   public void mouseClicked(MouseEvent e) {
     System.out.println("Mouse Clicked!");
	 flag = 1;
	 locationX = e.getX();
	 locationY = e.getY();
	 return;
   }		 
   
   public void mouseEntered(MouseEvent e) {
     //System.out.println("Mouse Entered!");
   }
   
   public void mouseExited(MouseEvent e) {
     //System.out.println("Mouse Exited!");
   }
   
   public void mousePressed(MouseEvent e) {
     //System.out.println("Mouse Pressed!");
   }
   
   public void mouseReleased(MouseEvent e) {
     //System.out.println("Mouse Released!");
   }
   
}

class MyThread extends Thread { 
    
	 String file = null;
	 int v = 0;
	
	 public MyThread() { }
     public MyThread(String nextFile) {
	     file = nextFile;
     }
	
	 public void run() {
         System.out.println("MyThread running");
         String metaFile = file.substring(0, file.length() - 3) + "meta";
	     
		 try {
	         audioReader.mdf = new metaDataFile(metaFile);
	     } catch (FileNotFoundException e) {
             audioReader.count = 0;
			 audioReader.mdf = null;
         } catch (IOException e) {
             e.printStackTrace();
         }
	   
		 if(audioReader.prevFlag == 0 && audioReader.nextFlag == 0) { 
			 Arrays.fill(audioReader.prevNextBuffer, audioReader.moveptr+1, 24, null);
		     audioReader.addptr = audioReader.moveptr;
			 audioReader.prevNextBuffer[++audioReader.addptr] = file;
	         audioReader.moveptr = audioReader.addptr;
			 audioReader.button2.setEnabled(false);
			 audioReader.button1.setEnabled(true);
		 } else {
		     audioReader.prevFlag = 0;
			 audioReader.nextFlag = 0;
		 }
		 
		 audioReader.videoLoader(file);	
		 System.out.println("loaded");	      
     }
}

class MyAudioThread extends Thread { 
    
	 String audioFile = null;
	 int srtFrame = 0;
	
	 public MyAudioThread() { }
     public MyAudioThread(String nextAudioFile, int frame) {
	     audioFile = nextAudioFile;
		 srtFrame = frame;
     }
	 
	 public void run() {
		 System.out.println("MyAudioThread running");
		 
		 while(audioReader.syncFlag == 0) {
		     //do nothing
		 }
		 
		 audioReader.syncFlag = 0;
		  
		 try {
             File soundFile = new File(audioFile);
		     audioReader.clip = AudioSystem.getClip();
	         AudioInputStream ais = AudioSystem.getAudioInputStream(soundFile);
	         audioReader.clip.open(ais);
			 audioReader.clip.setMicrosecondPosition(1000000 * srtFrame / 24);
			 audioReader.clip.loop(Clip.LOOP_CONTINUOUSLY);
         } catch (Exception e) {
             e.printStackTrace();
         }	      
	 }
}