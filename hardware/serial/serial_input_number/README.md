# Serial Monitor - Input Number

This program demonstrates how Arduino can receive numbers from a user through the Serial Console.

There are three main problems that are encountered with the naive solution to this code.

The first problem is that the Arduino keeps thinking you entered `0`, when you really didn't. It keeps outputting the following every second:

```
Enter a whole number, followed by !, then press enter: You input: 0
Enter a whole number, followed by !, then press enter: You input: 0
Enter a whole number, followed by !, then press enter: You input: 0
Enter a whole number, followed by !, then press enter: 
... (repeat every 1 second) ...
```

The problem here is that, in order to read a number, the Arduino has to recieve several characters in a row. For example, suppose you typed `123`. The Arduino would recieve a `1`, then a `2`, then a `3`. But how does it know that there isn't a `4` comming next, that you really meant to type `123` and not `1234`? It doesn't, so it waits for 1000 milliseconds.

Unfortunately, 1000 milliseconds is a bit too short. So, the solution is to tell the Arduino to wait longer. Much longer. In `setup()` we add a call to [`setTimeout`](http://arduino.cc/en/Serial/SetTimeout)

```
Serial.setTimeout(1000 * 60 * 60);
```

The second problem is a blank Serial Monitor. This occurs because the prompt is sent as soon as the program runs `loop()` for the first time, and the prompt gets lost because no Serial Monitor is listening for it yet. This happens milliseconds after program download, which is too fast for a user to open a prompt.

The solution is to add a delay in `setup()`, slowing down the Arduino and giving the user time to open a prompt.

```
delay(5000);
```

The third problem is a consequence of the first solution. Now that we've told the Arduino to wait for a (long) while, how does it know when the last digit of the number has been received? According to the `parseInt()` [documentation](http://arduino.cc/en/Serial/ParseInt), it will consider a number done when it gets a non-digit character. So, we modify the prompt to tell the user to type a `!` after the number:

```
Serial.print("Enter a whole number, followed by !, then press enter: ");
```

Note that an enter press would also count, but by default the Arduino Serial Monitor does not send it. You can change this behavior with the dropdown on the bottom of the Serial Monitor.

