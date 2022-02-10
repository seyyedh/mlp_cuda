#include <stdio.h>
#include <math.h>
#include "pgmimage.h"
#include "backprop.h"
#include <stdlib.h>
#include <string.h>
//#include <conio.h>

#define Threshold		0.5
#define NumberOfOutputs 7
#define NumberOfHiddens 40
#define LEARNING_RATE	0.2
#define MOMENTUM		0.8
#define	TRAINING_EPOCHS	300
#define SAVE_DELTA		1
// #define EPOCHS_AFTER_COMPLETELY_TRAINED	50

FILE *Internal_Activity_File;
FILE *ErrorFile;

extern char *strcpy();
extern void exit();

/* Forward declarations */
void backprop_face(
    IMAGELIST *trainlist, IMAGELIST *test1list, IMAGELIST *test2list,
    int epochs, int savedelta, 
    char *netname,
    int list_errors);
void performance_on_imagelist(
    BPNN *net,
    IMAGELIST *il,
    int list_errors);
int evaluate_performance(
    BPNN *net,
    double *err);

int main(void)
{
	int argc;
	char argv[22][20];
	char netname[256], trainname[256], test1name[256], test2name[256], errorname[256];
	IMAGELIST *trainlist, *test1list, *test2list;
	int ind, epochs, seed, savedelta, list_errors;

	argc=22;
	argv[1][0]='-';
	argv[1][1]='n';
	strcpy(argv[2], "AANN.txt");//network file

	argv[3][0]='-';
	argv[3][1]='e';
	strcpy(argv[4], "100000");//number of epochs

	argv[5][0]='-';
	argv[5][1]='s';
	strcpy(argv[6], "140694");//random number generator seed

	argv[7][0]='-';
	argv[7][1]='S';
	strcpy(argv[8], "1000");//number of epochs between saves of NN

	argv[9][0]='-';
	argv[9][1]='t';
	strcpy(argv[10], "trainlist.txt");//training set list

	argv[11][0]='-';
	argv[11][1]='1';
	strcpy(argv[12], "testlist.txt");//testing set 1 list

	argv[13][0]='-';
	argv[13][1]='2';
	strcpy(argv[14], "aatest2.txt");//testing set 2 list

	//argv[15][0]='-';
	//argv[15][1]='T';//list errors

	seed = 5000;   // today's date seemed like a good default //
	//epochs = 100;
	savedelta = 100;
	list_errors = 0;
	netname[0] = trainname[0] = test1name[0] = test2name[0] = errorname[0] = '\0';

	if (argc < 2)
	{
		exit (-1);
	}

	// Create imagelists //
	trainlist = imgl_alloc();
	test1list = imgl_alloc();
	test2list = imgl_alloc();

	// Scan command line //
	for (ind = 1; ind < argc; ind++)
	{
	// Parse switches //
	if (argv[ind][0] == '-')
	{
	  switch (argv[ind][1])
	  {
		case 'n': strcpy(netname, argv[++ind]);						break;
		case 'e': epochs = atoi(argv[++ind]);						break;
		case 's': seed = atoi(argv[++ind]);							break;
		case 'S': savedelta = atoi(argv[++ind]);					break;
		case 't': strcpy(trainname, argv[++ind]);					break;
		case '1': strcpy(test1name, argv[++ind]);					break;
		case '2': strcpy(test2name, argv[++ind]);					break;
		case 'T': list_errors = 1;   epochs = 0;				    break;
		default : printf("Unknown switch '%c'\n", argv[ind][1]);	break;
	  }
	}
	}
	epochs=TRAINING_EPOCHS;
	savedelta=SAVE_DELTA;

	//sprintf(netname,"AANet(H#=%d,E#=%d,ETA=%1.1f,MOM=%1.1f).txt", NumberOfHiddens, epochs, LEARNING_RATE, MOMENTUM);
	sprintf(errorname,"AAError(H#=%d,E#=%d,ETA=%1.1f,MOM=%1.1f).txt", NumberOfHiddens, epochs, LEARNING_RATE, MOMENTUM);
	ErrorFile=fopen(errorname,"w");

	// If any train, test1, or test2 sets have been specified, then load them in. //
	if (trainname[0] != '\0')
	imgl_load_images_from_textfile(trainlist, trainname);
	if (test1name[0] != '\0')
	imgl_load_images_from_textfile(test1list, test1name);
	// if (test2name[0] != '\0')
	// imgl_load_images_from_textfile(test2list, test2name);

	// If we haven't specified a network save file, we should... //
	if (netname[0] == '\0')
	{
	printf("%s: Must specify an output file, i.e., -n <network file>\n",
	 argv[0]);
	exit (-1);
	}

	// Don't try to train if there's no training data //
	if (trainname[0] == '\0')
	epochs = 0;

	// Initialize the neural net package //
	bpnn_initialize(seed);

	// Show number of images in train, test1, test2 //
	printf("%d images in training set\n", trainlist->n);
	printf("%d images in test1 set\n", test1list->n);
	// printf("%d images in test2 set\n", test2list->n);

	// If we've got at least one image to train on, go train the net //
	backprop_face(trainlist, test1list, test2list, epochs, savedelta, netname,list_errors);

    fclose(ErrorFile);

	printf("FINISHED, Press any key to continue!");
	//getch();
	exit(0);

	return 0;
}


void backprop_face(
    IMAGELIST *trainlist, IMAGELIST *test1list, IMAGELIST *test2list,
    int epochs, int savedelta,
    char *netname,
    int list_errors)
{
  IMAGE *iimg;
  BPNN *net;
  int train_n, epoch, i, imgsize;
  double out_err, hid_err, sumerr;
  int f=0;

  train_n = trainlist->n;

  // Read network in if it exists, otherwise make one from scratch //
  if ((net = bpnn_read(netname)) == NULL) {
    if (train_n > 0) {
      printf("Creating new network '%s'\n", netname);
      iimg = trainlist->list[0];
      if(f==0){
    	  printf("nc:%d, nr:%d", COLS(iimg), ROWS(iimg));
    	  f=1;
      }
      imgsize = ROWS(iimg) * COLS(iimg);
      /* bthom ===========================
	  make a net with:
	  imgsize inputs, 4 hiden units, and 1 output unit
          */
      net = bpnn_create(imgsize, NumberOfHiddens, NumberOfOutputs);
    } else {
      printf("Need some images to train on, use -t\n");
      return;
    }
  }

  // Print out performance before any epochs have been completed. //
  printf("\nepoch	Error	Train%%	TrErr	Test1%%	Test1Err\n");
  fprintf(ErrorFile,"epoch	Error	Train%%	TrErr	Test1%%	Test1Err\n");

  printf("0	0.0	");
  fprintf(ErrorFile,"0	0.0	");

  performance_on_imagelist(net, trainlist, 0);
  performance_on_imagelist(net, test1list, 0);
  // performance_on_imagelist(net, test2list, 0);
  printf("\n");  fflush(stdout);
  fprintf(ErrorFile,"\n"); fflush(ErrorFile);

  if (list_errors) {
    printf("\nFailed to classify the following images from the training set:\n");
    performance_on_imagelist(net, trainlist, 1);
    printf("\nFailed to classify the following images from the test set 1:\n");
    performance_on_imagelist(net, test1list, 1);
    printf("\nFailed to classify the following images from the test set 2:\n");
    // performance_on_imagelist(net, test2list, 1);
  }

  //*********** Train it **************************//
  /*
  for (epoch = 1; epoch <= epochs; epoch++) {

    printf("%d	", epoch);  fflush(stdout);
	fprintf(ErrorFile,"%d	", epoch);

    sumerr = 0.0;
    for (i = 0; i < train_n; i++) {

      // Set up input units on net with image i //
      load_input_with_image(trainlist->list[i], net);

      // Set up target vector for image i //
      load_target(trainlist->list[i], net);

      // Run backprop, learning rate 0.3, momentum 0.3 //
      bpnn_train(net, LEARNING_RATE, MOMENTUM, &out_err, &hid_err);

      sumerr += (out_err + hid_err);
    }
    printf("%3.3f	", sumerr);
	fprintf(ErrorFile,"%3.3f	", sumerr);

    // Evaluate performance on train, test, test2, and print perf //
    performance_on_imagelist(net, trainlist, 0);
    performance_on_imagelist(net, test1list, 0);
    // performance_on_imagelist(net, test2list, 0);
    printf("\n");  fflush(stdout);
	fprintf(ErrorFile,"\n");

    // Save network every 'savedelta' epochs //

   // if (!(epoch % savedelta)) {
     // bpnn_save(net, netname);
   // }

  }
  */
  printf("\n"); fflush(stdout);
  fprintf(ErrorFile,"\n");

  // Save the trained network //
  /*
   if (epochs > 0) {
     bpnn_save(net, netname);
   }
   */
}


// Computes the performance of a net on the images in the imagelist. //
// Prints out the percentage correct on the image set, and the
//average error between the target and the output units for the set. //
void performance_on_imagelist(
    BPNN *net,
    IMAGELIST *il,
    int list_errors)
{
  double err, val;
  int i, n, j, correct;

  err = 0.0;
  correct = 0;
  n = il->n;
  if (n > 0) {
    for (i = 0; i < n; i++) {

      // Load the image into the input layer. **/
      load_input_with_image(il->list[i], net);

      // Run the net on this input. **/
      bpnn_feedforward(net);

      // Set up the target vector for this image. **/
      load_target(il->list[i], net);

      // See if it got it right. //
      if (evaluate_performance(net, &val)) {
        correct++;
      } else if (list_errors) {
		printf("%s - outputs ", NAME(il->list[i]));
		for (j = 1; j <= net->output_n; j++) {
		  printf("%.3f ", net->output_units[j]);
		}
		putchar('\n');
      }
      err += val;
    }

    err = err / (double) n;

    if (!list_errors){
      printf("%3.2f	%3.4f	", ((double) correct / (double) n) * 100.0, err);
	  fprintf(ErrorFile,"%3.2f	%3.4f	",((double) correct / (double) n) * 100.0, err);
	}
	} else {
      if (!list_errors)
        printf("0.0 0.0 ");
	}
}

int evaluate_performance(
    BPNN *net,
    double *err)
{
  double delta;
  int Flag,i;

  Flag=1;
  *err=0.0;

  for(i=1;i<=NumberOfOutputs;i++)
  {
	  delta = net->target[i] - net->output_units[i];

	  *err =*err + (0.5 * delta * delta);

	  // If the target unit is on... //
	  if (net->target[i] > Threshold)
	  {
		// If the output unit is on, then we correctly recognized me! //
		if (net->output_units[i] > Threshold)
		  Flag=Flag && 1;
		// otherwise, we didn't think it was me... //
		else
		  Flag=Flag && 0;

	  // Else, the target unit is off... //
	  }
	  else
	  {
		// If the output unit is on, then we mistakenly thought it was me //
		if (net->output_units[i] > Threshold)
		  Flag=Flag && 0;
		// else, we correctly realized that it wasn't me //
		else
		  Flag=Flag && 1;
	  }
  }//end for i
  return Flag;

}
