public abstract class Media // abstract media class
{
    private String name; // name of media
    private String other_info; // other information

    public Media (String name, String other_info) // constructor
    {
        this.name = name;
        this.other_info = other_info;
    }
    public String get_name () // returns name
    {
        return name;
    }
    public String get_other_info () // returns other information
    {
        return other_info;
    }

    public abstract void info ();
}

