HOMEWORK 10: DISTANCE FIELDS & FAST MARCHING METHOD


NAME:  < Grey Ding >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

mentor:YiZhou Guo,Shelly,Sutherland ,Connor,Jason
classmates:Qi Jin; Dong Lin;WeiJun Li;ZiJian Zhong;ZeYu Ren;YiFan Wang;
ChenHao Tan; JiaWei Zhao
cplusplus.com

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 10 >



NAIVE ALGORITHM:

Big `O' Notation: w^2 * h^2

Timing Experiment Data: average: 0.023s  
time ./distancefield.out squiggle_30x30.ppm out.ppm naive_method rainbow

Discussion:
this function use four foor loops to find the current pixel and to compare with the black pixels,
and it loop over the height and width twice, so the time performace is bad.


IMPROVED ALGORITHM:

Big `O' Notation: w*h + w*h*n(size of the black pixel vector)

Timing Experiment Data: average:0.016s
time ./distancefield.out squiggle_30x30.ppm out.ppm improved_method rainbow

Discussion:
this fuction first use two for loops to find all the black pixels,
and then it use another two for loop to find current pixel,then it only need
to loop over the vector of the black pixels and calcualte the distance of that 
two pixels, so the time performace is a little bit better, but its still a complicate
one (O(n^3))



FAST MARCHING METHOD (with a map):

Big `O' Notation: O(w*h + n.size*w*h + 9n)

Timing Experiment Data: 0.09s
time ./distancefield.out squiggle_30x30.ppm out.ppm pq_with_map rainbow

Discussion:
this fuction first use two for loops to find all the black pixels, and then it
find all the pixels that are next to black pixels and add them to the queue,
then use a while loop to loop over the queue, and pop the front pixel, and set its color to blue.



DISTANCE FIELD VISUALIZATIONS FOR EXTRA CREDIT:




FAST MARCHING METHOD (with a hash table) FOR EXTRA CREDIT:

Big `O' Notation:

Timing Experiment Data:

Discussion:



MISC. COMMENTS TO GRADER:  
Optional, please be concise!






