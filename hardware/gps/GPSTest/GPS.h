

// set to 'true' if you want to print the raw GPS sentences to Serial console
bool print_raw_gps = false;

// set to 'true' if you want to print the parsed GPS info to Serial console
bool print_parsed_gps = true;

// initialize utility that will convert lat/lon to (x,y) positions in meters
UsefulCalcs calc(false);

// this will be false until GPS fix is found and starting position saved
bool start_pos_found = false;

// once GPS fix is found, these variables will be updated
float startPositionX = 0;
float startPositionY = 0;
float relPositionX = 0;
float relPositionY = 0;
float pos[2];
float angleFromStart = 0;
