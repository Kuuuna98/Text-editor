# Text Editor📑
Implement a text editor-based console.
![0](https://user-images.githubusercontent.com/26339069/104284612-a365a380-54f5-11eb-8822-182f953ae7a9.png)
## Functional specification
- Use design pattern **Singletone**.
- The "test.txt" file will be loaded when the program starts. Next, print the first page.
-  Maximum 20 lines, 75 byte(including spaces) per a page.
- The user enters the action to perform at the "입력" prompt. (The factors are separated by commas.)
- Spaces in parentheses treated as input errors.
- (콘솔메시지) displays a message about the exception situation for each input.
- The output of each line is adjusted according to the results of the insert, delete, and change operations.
- Except for keyword 't', It is re-printed after working according to user input.
- For 'p' and 'n', which are functions of turning pages, move the 20 line to the maximum unit. If there are two lines left to output, add two new lines to the current 18 lines (line number 3-20)  and output them to the screen.
- **Error handling**: I tried to prevent programs from terminating abnormally due to user input.

### Keywords and Examples
- n: next page  
  - ex) n
  ![1](https://user-images.githubusercontent.com/26339069/104285207-9ac19d00-54f6-11eb-9e59-62d4d81aacb5.png)
- p:  previous page
  - ex) p
  ![2](https://user-images.githubusercontent.com/26339069/104287090-4ec42780-54f9-11eb-95ed-bd37dd817bb8.png)
- i:  insert word
  - ex) i(1,10,hello)
  -- "hello" inserted in the 10th position on first line 
  ![3](https://user-images.githubusercontent.com/26339069/104287336-a5316600-54f9-11eb-9f50-c42e4a91a379.png)
- d:  delete word
  - ex) d(2,10)
 -- It deleted a word in the 10th position on the second line.
 ![4](https://user-images.githubusercontent.com/26339069/104287096-4ff55480-54f9-11eb-8556-b1b9de4b6c8f.png)
- c:  change word
  - ex) c(hello,bye)
 --It find all "hello" in "test.txt" and change to "bye".
 ![5](https://user-images.githubusercontent.com/26339069/104287098-508deb00-54f9-11eb-9a7e-19330bf3a9ce.png)
- s:  search word
  - ex) s(or)
 -- Re-print the screen so that the first "or" word found by navigating from the beginning of the file is first located on the first line of the output window.
 ![6](https://user-images.githubusercontent.com/26339069/104287100-51bf1800-54f9-11eb-9131-82d816e32716.png)
- t:  exit program after save file
  - ex) t
  ![7](https://user-images.githubusercontent.com/26339069/104287088-4d92fa80-54f9-11eb-83ae-d99e59cfcf9d.png)



## ClassDiagram
![다이어그램](https://user-images.githubusercontent.com/26339069/104282116-e6257c80-54f1-11eb-8735-b6648b37a177.png)
