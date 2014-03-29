
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

public class videoReader {

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
	//videoLoader(fileName);
	
	//sound test
	try {
         File soundFile = new File("vdo1.wav");
		 Clip clip = AudioSystem.getClip();
	     AudioInputStream ais = AudioSystem.getAudioInputStream(soundFile);
	     clip.open(ais);
	     //clip.setMicrosecondPosition(1000000/16);
		 clip.loop(Clip.LOOP_CONTINUOUSLY);
		 SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                // A GUI element to prevent the Clip's daemon Thread
                // from terminating at the end of the main()
                JOptionPane.showMessageDialog(null, "Close to exit!");
            }
         });
    } catch (Exception e) {
         e.printStackTrace();
    }
	
	//videoPlayer(img);
	
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
				 g.drawRect(tlx,tly,brx,bry);
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
   
    int frameRate = 16;
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
		 f.getContentPane().remove(1);
		 MyThread pThread = new MyThread(prevNextBuffer[--moveptr]);
         pThread.start();
		 
		 if(moveptr == 0)
		     button1.setEnabled(false);
			 
		 try {
             Thread.sleep(2500);
         } catch(InterruptedException ex) {
             Thread.currentThread().interrupt();
         }
		 
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 timer.schedule(t, 0, duration);
		 
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
		 f.getContentPane().remove(1);
		 MyThread nThread = new MyThread(prevNextBuffer[++moveptr]);
         nThread.start();
		 
		 if(moveptr == addptr) 
		     button2.setEnabled(false);
		 
		 try {
             Thread.sleep(2500);
         } catch(InterruptedException ex) {
             Thread.currentThread().interrupt();
         }
		 
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 timer.schedule(t, 0, duration);
     }
   }
   
   static class ActionPlay implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Play!"); 		 
		 button3.setEnabled(false);
		 f.getContentPane().remove(1);
		 timer = new Timer();
		 t = new Task(img,f,curFrame);
		 timer.schedule(t, 0, duration);		 
     }
   }
   
   static class ActionPause implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Pause!");
         button3.setEnabled(true);
		 curFrame = t.c;
		 timer.cancel();
         timer.purge();		 
     }
   }
   
   static class ActionStop implements ActionListener {        
     public void actionPerformed (ActionEvent e) {     
         System.out.println("Clicked Stop!");
         button3.setEnabled(true);
		 curFrame = 0;
		 timer.cancel();
         timer.purge();		 
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
		 videoReader.count = 0;
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
				 id[videoReader.count] = s.nextInt();
				 name[videoReader.count] = s.next();
				 numKeyFrames[videoReader.count] = s.nextInt();
				 linkedVideo[videoReader.count] = s.next();
				 linkedVideoFrame[videoReader.count] = s.nextInt();
				 
				 for(i = 0; i < numKeyFrames[videoReader.count]; i++) {
				     keyFrame[videoReader.count][i] = s.nextInt();
					 topLeftX[videoReader.count][i] = s.nextInt();
					 topLeftY[videoReader.count][i] = s.nextInt();
					 bottomRightX[videoReader.count][i] = s.nextInt();
					 bottomRightY[videoReader.count][i] = s.nextInt();
				 }
				 
                 videoReader.count++;
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
		 flag = 0;
		 frame.remove(label);
		 
		 //check if clicked location is within bounding box of any hyperlink
		 for(u = 0; u < videoReader.count; u++) { 
		     if(c > videoReader.mdf.keyFrame[u][0] && c < videoReader.mdf.keyFrame[u][videoReader.mdf.numKeyFrames[u]-1]) {
		         videoReader.interpolate(c,u);
		         if(locationX >= videoReader.tlx && locationX <= videoReader.brx && locationY >= videoReader.tly && locationY <= videoReader.bry) {
                     falseAlarm = false;
                     break;
                 }				 
		     }
		 }
		 
		 if(falseAlarm == false) {
		     falseAlarm = true;
			 int startFrame = videoReader.mdf.linkedVideoFrame[u];
			 MyThread myThread = new MyThread(videoReader.mdf.linkedVideo[u]);
             myThread.start();
		 
		     try {
                 Thread.sleep(2500);
             } catch(InterruptedException ex) {
                 Thread.currentThread().interrupt();
             }
		 
			 videoReader.timer = new Timer();
		     videoReader.t = new Task(videoReader.img,frame,startFrame);
		     videoReader.timer.schedule(videoReader.t, 0, 1000/16);
	  
	     } else {
		     videoReader.timer = new Timer();
		     videoReader.t = new Task(videoReader.img,frame,c);
		     videoReader.timer.schedule(videoReader.t, 0, 1000/16);
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
	         videoReader.mdf = new metaDataFile(metaFile);
	     } catch (FileNotFoundException e) {
             videoReader.count = 0;
			 videoReader.mdf = null;
         } catch (IOException e) {
             e.printStackTrace();
         }
	   
		 if(videoReader.prevFlag == 0 && videoReader.nextFlag == 0) { 
			 Arrays.fill(videoReader.prevNextBuffer, videoReader.moveptr+1, 24, null);
		     videoReader.addptr = videoReader.moveptr;
			 videoReader.prevNextBuffer[++videoReader.addptr] = file;
	         videoReader.moveptr = videoReader.addptr;
			 videoReader.button2.setEnabled(false);
			 videoReader.button1.setEnabled(true);
		 } else {
		     videoReader.prevFlag = 0;
			 videoReader.nextFlag = 0;
		 }
		 
		 videoReader.videoLoader(file);	
		 System.out.println("loaded");	      
     }
}