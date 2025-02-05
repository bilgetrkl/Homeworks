public class Audio extends Media implements Playable, Non_visual // audio class
{
    private String duration; // duration of the audio

    public Audio (String name, String duration, String other_info) // constructor
    {
        super (name, other_info); // calls superclass constructor
        this.duration = duration; 
    }
    @Override
    public void info () // prints the information of the audio
    {
        System.out.println (get_name () + " " + duration + " " + get_other_info ());
    }
    @Override
    public void play () // plays the audio
    {
        System.out.println ("Playing audio: " + get_name ());
    }
    @Override
    public void view ()
    {
        play ();
    }
}
