
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Task Type: HSHEX MONO COMP*/

/* The RGB values of a pixel. */
struct Pixel {
   
    unsigned short red;   
    
    unsigned short blue;   

    unsigned short green;
};

struct Image {
    int width;           
    /*Width of the image, represented by an integer number of columns*/
    int height;         
    /*Height of the image, represented by an integer number of rows*/
    struct Pixel *pixels; 
    /*Pointer to a dynamically-allocated array of Pixels, that we have 
    defined above*/
};

struct MultipleImages {
    struct Image *currentImage;
    struct MultipleImages *nextImage; 

};


/* Free a struct Image */
void free_image(struct Image *img)
{
    if (img != NULL) {
        /*Firstly, we check if the image pointer is not null, to prevent dereferencing 
        an invalid pointer, that may lead to system crashes*/
        
        if (img->pixels != NULL) {
        /*We then checki if the pixel pointer "(img->pixels)"" is not NULL to ensure that you only attempt to free memory 
        if it has been properly allocated to avoid undefined behavior*/
           free(img->pixels);
            /*We first free the dynamically-allocated array of pixels, by accesseing the "pixels" component of the "struct Image" 
            that "Img" has pointed to, assuming that "img" is a valid pointer to "struct Image". We use the "free" function to 
            ensure that all memory that the array has occupied is released*/ 
        }

        free(img);
        /*We finally release the memory for both the pixel data and struct Image itself*/
    }
}


/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
{
   
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }
     /*Opening our file for loading, using standard file opening C functions. We refer to the file as 'r' */

    /* Allocate the Image object, and read the image from the file */
    
    struct Image *img = malloc(sizeof(struct Image));
   /*The "malloc" function in C is used to dynamically allocate space of memory for a type.
   We determine the size required to store the object of type "struct Image" in this case through
   "sizeof" and then dynamically allocating a memory to allow us to create objects with the size
   determined at runtime, making sure memory is assigned beyond the current function.*/
    
    
    if (fscanf(f, "HSHEX %d %d", &img->width, &img->height) != 2) {
    /*In order to read the header information, we use the scanf feature to read exactly 2 values, the width
     and height integers from the file stream that we called 'f', to make sure our header information is in 
     the expected format*/

        fprintf(stderr, "Error: invalid HSHEX header.\n");
        /*If the header format in invalid, an error message is printed*/
        
        free(img);
        /*Freeing the memory that we previously allocated through "malloc" before exiting the function upon any error*/
      
        return NULL;
        /*Returning null to announce that an error has occured*/
    }

   
    img->pixels = malloc(img->width * img->height * sizeof(struct Pixel));
    /*Here, we dynamically allocate memory for the pixel data of our image, through using the malloc function again. We
    determine the size of the memory block based on the width, height and size per pixel.*/
   
    
    for (int i = 0; i < img->width * img->height; ++i) {
     /*As we need to iterate through each pixel, we create a for loop to go through each pixel present in the image*/
        if (fscanf(f, "%hx %hx %hx", &img->pixels[i].red, &img->pixels[i].blue, &img->pixels[i].green) != 3) {
         /*As the type specification for this specific project is HSHEX, meaning we have hexadecimal values to represent 
         values, we use "fscanf" to read three int values from the file for each pixel, represented by a "%d" format
         specifier. We check for three values, since tour type consists of "red","green" and "blue".*/   
            
            fprintf(stderr, "Error: Trouble reading pixel data.\n");
            /*If the above process was unsucessful, we print an error message.*/
           
            free(img->pixels);
            /*If the process did not work out, we free up the space previously allocated for this function*/
            
            return NULL;
            /*Returning null if an error occurs*/
        }
    }
    /*Reading our pixel data*/

   
   if (img->pixels == NULL) {
    /*We check if memory allocation has been unsuccessful, through checking if the allocation is null. */
        fprintf(stderr, "Error: Memory allocation unsuccessful.\n");
        /*Error message for failed memory allocation attempt*/

        free(img);
        /*Freeing memory previously assigned*/
        
        return NULL;
        /*Returning null to indicate error*/
    }

    /* Close the file */
    fclose(f);

 if (img == NULL) {
    /*Checking if our point "img" is null, to indicate the attempt at memory allocation done prior 
    (using the malloc function) has failed*/
        fprintf(stderr, "File %s could not be read.\n", filename);
        /*Printing an error message*/
        return NULL;
        /*Returning null, since the memory allocation has failed*/
    }

    return img;
}

/* Write img to file filename. Return true on success, false on error. */
bool save_image(const struct Image *img, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }
    /*Opening our file for saving, using standard file opening C techniques. We refer to the file as 's' */

/*Writing the contents of our file:*/

    fprintf(f, "HSHEX %d %d\n", img->width, img->height);
    /*Printing the header information, using the %d format specifiers to represent the width and height of our image
    to be saved, that have specified as "img->width" and "img->height"*/

    for (int i = 0; i < img->width * img->height; ++i) {
    /*Iterting over each pixel in the image, with reference to height and width to assist in determining the individual 
    pixels located in the image, acting as (x,y) coordinates. We use 'i' as a loop counter index to access
    every pixel present in the array of pixels.*/
     fprintf(f, "%hx %hx %hx\n", img->pixels[i].red, img->pixels[i].blue, img->pixels[i].green);
     /*In this line of code, we write our 3 hexadecimal values for every pixel, using "%hx" as a format specifier. The values
     printed are the "HEX","Mono" and "Comp" values, that are seperated with spaces and followed by a newline character.*/
    }
    /*We write the pixel data on the file, as it is vital to store visual properties, as one of the contents of image.*/


    return true;
    /*Returning true to indicate successful saving*/

}

/* Allocate a new struct Image and copy an existing struct Image's contents
 * into it. On error, returns NULL. */
struct Image *copy_image(const struct Image *source)
{
   if (source == NULL) {
    return NULL;
    }
    /*If image source is null, meaning the source image does not exist or is invalid, it returns "NULL" to indicate error*/

    struct Image *newImage = malloc(sizeof(struct Image));
    /*We allocate memory for the new image, similar to what we did in the "load_image" function above*/

    newImage -> width = source -> width;
    newImage -> height = source -> height;
    /*We copy the height and width to ensure we have an accurate representation of the dimensions of the image, to 
    obtain accurate pixel data and be utilised in our dynamic memory location later on.*/ 

    newImage->pixels = malloc(source->width * source->height * sizeof(struct Pixel));
    /*Next, we allocate memory for the new image's pixel array. Firstly, we access the "pixels" component of the 
    "newImage" structure, that inherently holds the address of the dynamicallt allocated memory block, posing as a 
    pointer. Then, we calculate the size that is needed to store the pixel data for the new image- obtaining the total
    number of pixels by multiplying the width and height, before multiplying this by "sizeof(struct Pixel)" to get the 
    total byte size, using our standard "malloc" function to perform memory allocation. Furthermore, to inform the 
    compiler about the type of data that will be stored in the allocated memory, we employ type-casting to a pointer to
    "(struct Pixel*)", since the return value of "malloc" is intially "void". Overall, this line of code essentially 
    guarantees that the new image successfully accomadates the same number of pixels as our initial source image to
    produce an accurate copy.*/
    
    for (int i = 0; i < source->width * source->height; ++i) {
        newImage->pixels[i] = source->pixels[i];
    }
    /*We apply a for loop to copy the pixel data from our "source" Image to our "newImage", where our loop counter 
    initializes at 0 and increments by 1. The line of code inside directly copies the pixel at positio "i" to it's
    respective position in the "newImage". We do this element by element, through referring to the ith element in each
    pixel of array of both the "source" Image and "newImage", arraning it in a manner where every pixel in "newImage" is 
    the same as "source" Image.*/

    if (newImage->pixels == NULL) {
        free(newImage); 
        return NULL;     
    }
    /*An error condition for if the dynamic memory allocation was successful, through checking if it is null,
    before freeing the space where memory was previously assigned and returning "NULL" as error.*/

    if(newImage==NULL) {
        return NULL;
    }
    /*If memory allocation for the new image, we return NULL for an error*/

}

/* Perform your first task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns a new struct Image containing the result, or NULL on error. */
struct Image *apply_MONO(const struct Image *source)
{
    struct Image *monoImage = malloc(sizeof(struct Image));
    /*Using the "malloc" function to allocate memory for our outpute image, with the size calculated of a 
    single instance of "struct Image", that consists of it's width, height and the included pointer to our 
    pixel data array. Now, our "monoimage" has been dynamically assigned a memory space.*/

    monoImage -> width = source->width;
    monoImage -> height = source->height;
    /*We copy the width and height of our "source" Image. We use an asterisk symbol ("*") in an attempt to 
    dereference the pointer "monoImage", to access the width and heigh of the structure being pointed to by 
    "monoImage". If we do not use this, we will be attempting to access the members of the pointer variable, 
    which we do not want to do. */

    monoImage->pixels = malloc(source->width * source->height * sizeof(struct Pixel));
    /*We now allocate the memory for our pixel data in the output image. We first access the "pixels" section of
    the structure referred to by "monoImage", through dereferencing it to gain access to the actual structure. We
    then traditionally allocate memory for our pixel data with the standard measurements of width, height and the 
    size of each pixel.*/
   
    for (int i = 0; i < source->width * source->height; ++i) {
    /*Using a for loop to iterate through every pixel in the image.*/
        
        int grayValue = (int)(0.299 * source->pixels[i].red +
                               0.587 * source->pixels[i].green +
                               0.114 * source->pixels[i].blue);
        /*We now caluclate the weighted sum of the red, blue and green components with the greyscale value, defined
        in the coursework specifications as floating point values. After individually applying the greyscale to each
        red, green and blue component, we add them up to obtain a weighted sum and cast the value to an integer. This
        is done because our pixel values are represented as integers, maintining the overall consistency, whilst 
        maintaining the accuracy to the best degree possible, as well as preveting integer wraparound as required.*/
    
        monoImage->pixels[i].red = grayValue;
        monoImage->pixels[i].green = grayValue;
        monoImage->pixels[i].blue = grayValue;
        /*Now, we set every red, green and blue pixel of our final "monoImage" to the "greyValue" we have calculated to
        apply the "MONO" effect to our image*/

    }
    

    

    if (source == NULL) {
        return NULL; 
    }
    /*If the image source is invalid, we check for "NULL" and return "NULL" to signal error.*/

    if (monoImage == NULL) {
        return NULL; 
    } /*If the memory allocation of our "monoImage" was unsucessful, we return "NULL" as an error.*/
    
    if (monoImage->pixels == NULL) {
        return false;
    /*If the memory allocation for the pixel data does not work out, we free the space and return "NULL" as an 
    indicator of error.*/
    }

    return monoImage;
    /*We return our final image, once we have applied the greyscale effect and and addressed potential errors.*/

}

/* Perform your second task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns true on success, or false on error. */
bool apply_COMP(const struct Image *source, const struct Image *reference)
{

    int identicalPixels = 0;
    int differentPixels = 0;
    /*Initialize integer counters for the dentical and different pixels, between the source and reference and images*/

    for (int i = 0; i < source->width * source->height; ++i) {
    /*Constructing a for loop to iterate through the pixels to compare their RGB values and categorize them as either 
    identical or different */ 
        if (source->pixels[i].red == reference->pixels[i].red &&
            source->pixels[i].green == reference->pixels[i].green &&
            source->pixels[i].blue == reference->pixels[i].blue) {
        /*Checking if the pixels are identical by checking if the values for the reference and source images are all equal
        for every component of RGB(Red, Green, Blue).*/
            
            identicalPixels++;
            /*Incrementing our "identicalPixels" counter*/

        } else {
            differentPixels++;
            /*If the above condition has not been satisfied, it suggests the pixels are different, therefore incrementing our
            "differentPixels" counter*/
        }
    }

    printf("Identical Pixels: %d\n", identicalPixels);
    printf("Different Pixels: %d\n", differentPixels);
    /*Printing the Identical and Different pixel values on different lined ("/n"), using "%d" as a format specifiers with a
    reference to the integer values from "int identicalPixels" and "int differentPixels"*/

    return true;
    /*Returning true if the above is followed, and no errors have been encountered.*/


    /*We now specify the potential error conditions in case something goes wrong in the system, or error in input with each of
    them returning false:*/
    
    if (source == NULL) {
    fprintf(stderr, "Error: Source image is NULL.\n");
    return false;
    }
    /*If the source image has a "NULL" reference, meaning the pointer hasn't been correctly initialized with a valid memory address,
    it will print an error message and return false.*/

    if (reference == NULL) {
    fprintf(stderr, "Error: Reference image is NULL.\n");
    return false;
    }
    /*If the reference image has a "NULL" reference, similary to our "source" reference above, indicating an error in input it will 
    print an error message and return false.*/

    if (source->width != reference->width || source->height != reference->height) {
    fprintf(stderr, "Error: Input images have different dimensions for comparison.\n");
    return false;
    }
    /*In this potential error condition, we check if the width and height of our source and reference images are equal. If they are not,
    we use the "NOT" symbol to check for each, as well as the "OR" symbol for either the height or the width being different, and then print
    an error message informing that they have different dimensions and are therefore invalid for comparison and then returning false. */

}

int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    /* TODO: Remove this if you don't need to use rand() */
    

    /* Check command-line arguments */
    if (argc < 3 || argc % 2 != 1) {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    for (int i = 1; i < argc; i+=2) {
        struct Image *in_img = load_image(argv[i]);
        if (in_img == NULL) {
            continue;
        }
    


   
    

    /* Apply the first process */
    struct Image *out_img = apply_MONO(in_img);
    if (out_img == NULL) {
        fprintf(stderr, "First process failed.\n");
        free_image(in_img);
        continue;;
    }

    if (!save_image(out_img, argv[i+1]))
    {
        fprintf(stderr, "Error: Failed saving image");
    }


    /* Apply the second process */
    if (!apply_COMP(in_img, out_img)) {
        fprintf(stderr, "Second process failed.\n");
        free_image(in_img);
        free_image(out_img);
        continue;
    }

    /* Save the output image */
    if (!save_image(out_img, argv[2])) {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    free_image(in_img);
    free_image(out_img);

    }
    return 0;
}
