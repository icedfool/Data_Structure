HOMEWORK 4: VISUAL DIFFERENCE LISTS


NAME:  < Grey Ding >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cplusplus.com
TA:kunyuan,Ben
mentor:shirley,yizhou guo,Sutherland,Connor
class mates:Qi Jin; Dong Lin;WeiJun Li;ZiJian Zhong;ZeYu Ren;

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 20 >



ORDER NOTATION:
Give the order notation for each function below assuming
  w words in the first input file, and requiring
  i inserts, e erases, and r replace operations.
Write one or two sentences justifying each answer.


compute_diff - (default, non-optimal, non-recursive version)
O(w), since i just used one for loop to go over the string.

apply_diff
O(w), since I use a while loop to go over the string list, and inside 
this list I tried to match the diff list, which do not duplicate the big O // edit
notation.

invert_diff
O(w), since I use a while loop to go over the string list, and inside 
this list I tried to match the diff list, which do not duplicate the big O //
notation.

assert_same
O(w), since I just used a for loop to go over the string list,and without a for loop inside.

assert_same_diff
O(w), since I just used a for loop to go over the string list,and without a for loop inside.

read_text
O(1), since it's only using if statement and while loop,and the while loop is just a cin, so the
bigO is just O(1).

write_text
O(w), since it is using a while to detect if the filestream is good, and that should loop
the whole string list, so the big O should be O(w).

read_diff
O(n^2),since it use 2 while loops, the first is from 124 to 155, and inner is 134-144.
write_diff

render_diff
O(w), since the outside is a for loop, and in render.cpp line62 there's a for loop, but that
braely matters

OPTIONAL EXTRA CREDIT - RECURSIVE SOLUTION FOR MINIMUM EDIT DISTANCE:

Include running times and interesting screenshots of the HTML
visualization for test cases you created to debug your solution.

What is the big notation of your function? (Use the same variables as
above)


compute_diff - (optimal, ecursive version)



MISC. COMMENTS TO GRADER:  
Optional, please be concise!
