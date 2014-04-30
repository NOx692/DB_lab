//import static img.ImageUtility.*;
import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.ImageIO;

public class RGB{
    public static void main(String[] args) throws IOException {
        File f = new File("cat.png");
        BufferedImage read=ImageIO.read(f);
        int w = read.getWidth(),h=read.getHeight();
        BufferedImage write =
                new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
        
	System.out.println(h + " " + w);
	
        for(int y=0;y<h;y++){
            for(int x=0;x<w;x++){
                int c = read.getRGB(x, y);
                int r = 255- c>>16&0xff;
                int g = 255- c>>8&0xff;
                int b = 255- c>>0&0xff;
                int rgb = 0xff000000 | r << 16 | g << 8 | b;
                write.setRGB(x,y,rgb);		
		System.out.println(r + " " + g + " " + b);		
	    }
	    System.out.println();
        }
        
        File f2 = new File("ret.png");
        ImageIO.write(write, "png", f2);
    }
}