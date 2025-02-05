public class Video extends Media implements Playable, Visual // video class
{
    private String duration; // duration of video

    public Video (String name, String duration, String other_info) // constructor
    {
        super (name, other_info); // calls superclass constructor
        this.duration = duration;
    }
    @Override
    public void info () // displays information
    {
        System.out.println (get_name () + " " + duration + " " + get_other_info ());
    }
    @Override
    public void play () // plays video
    {
        System.out.println ("Playing video: " + get_name ());
    }
    @Override
    public void display () // displays video
    {
        play ();
    }
}
