#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main() {
    FILE *file, *f;
    char line[100], l[100]; // buffers to move and copy lines in the file
    
    int count = 1, // this counts the line we're in right now 
        StartDelete = 6, //here the first line of the sequence that we're gonna delete
        EndDelete = 11, // the line where deleting end
        size = 10; // how many lines in the file

    file = fopen("data.txt", "w+"); // Open the file in read-write mode
    
    f = fopen("data.txt", "r+"); // open another handle for the file to skip the lines we wanna delete 

    if (file == NULL || f == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    //take user's input 
    for ( int i = 0; i < size; ++i ) {
        printf("name %d: ", i+1);
        assert( fgets( line, sizeof(line), stdin ) );
        fprintf( file, "%s%s", "name: ", line );
    }
    fputs("\n", file); // i added extra empty line for the case when we need to delete the last line
    
    
    rewind(file); // going back to the start of the file 
    
    //making the second file handle reach the first line after the sequence we want delete 
    while( count <= EndDelete ) {
        count++;
        fgets(l, sizeof(l), f);
    }
    
    count = 0;// reset the lines counter to zero this time because of the way fgets work
    
    // Read each line and overwrite the file with lines that are not marked for deletion
    while (fgets(line, sizeof(line), file)) { //moving to the lines in the file
    
        count++;//the line conter grows as we move forward in lines
        
        if (count > StartDelete) {
            fseek(file, -strlen(line), SEEK_CUR);//going back to the beginning of the line again because of the mechanism of fgets 
            
            fwrite(l, sizeof(char), strlen(l), file);//overwriting 
            
            if(fgets(l, sizeof(l), f));//moving to the next line we wanna overwrite with
            else break;
        }
    }

    // Truncate the file to remove any remaining content beyond the new end of file
    fseek(file, ftell(file), SEEK_SET);
    ftruncate(fileno(file), ftell(file)-2); // ftell(file)-2 in here cuz for some reason there will be two empty lines at the end

    fclose(file);
    fclose(f);

    return 0;
}
