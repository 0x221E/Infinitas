print("Check whether 5321 port on 127.0.0.1 is open.");

sandbox
{
    var request = network.send_raw(TCP, [ACK], 127.0.0.1, 5321).async();

    if (request.response.socket == [SYN])
    {
        print("Port is open!");
    }
}

/** This variable lives in the first map of the environment stack. */
var a = "This is a";

void a(){

    /** This variable lives in the second map of the environment stack. */
    var b = "test.";

    /** An unqualified name lookup will have to be performed to access "a". */
    /** The unqualified lookup will progress to the previous maps if not found in the current. */
    var b = a + " " + b;
    
}