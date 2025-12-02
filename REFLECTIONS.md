# Reflections

This was my proper c project I have completed since learning the basics of the language. When I started this project, I thought it would take a week or two weeks. My original plan was to write a simple API written in pure c with no dependencies for the AES algorithm and a few tests to verify it worked. I wanted iot to be clearly laid out and reasonably well tested and I wanted to be able to demonstrate it's functions with the test code. However, as I worked I became bored of the idea of a project with no usability to non programmers. I wanted to show my friends what I had been working on. This was the reason I then implemented the file utilities and CLI elements of the project. It's also for this reason I made it cross compilable for Windows and wrote an install script for it.
So my initial specifications were:  

- AES spec algorithm which can encrypt and decrypt a block
- Tests to demonstrate full functionality of each part of the program
- Good readability
- No dependencies  

Later the following specifications were added:

- File encryption
- CLI
- Windows Cross compatibility
- Easy install/use instructions

I think I achieved these aims with the exception of some of the test code and file utility's code readability. This came down to rushing to get a newly planned feature completed without planning it properly. I had to refactor the code in the file-utilities directory substantially and I am still not full satisfied with the way it works. Also, I did not consider runtime of the cipher function to be a priority when first starting the project, however once thousands of blocks were involved in the encryption of a JPEG for example, this became a clear bottleneck, also since committing to basic CBC eliminates the possibility of any kind of multithreading. The most obvious way to speed it up would be to rewrite core so that it operates on entire words as int32_t's for example. This could realistically produce at four times improvement. A parallelizable mode of operation would also speed up the process in a multi core cpu.

## Makefiles

Refactoring the Makefile system was also a large time consuming task. Originally I just had one very messy makefile. I slowly moved functionality to the sub level makefiles. During this process I had to learn how makefiles work - I had been winging it with minimal understanding up to this point. Setting up a well organized build system from the start will be a priority in my next project. I'm interested to try this CMake thing.

## Using AI

I have mixed feelings about using AI in programming. It almost feels like you can't avoid it these days. I used AI in various ways:

- asking it questions to improve my conceptual understanding
- asking it to write test code(using an example test function) and helper/debugging print functions
- asking it if it could see obvious bugs
- asking to make codebase wide changes to function arguments, names
- It helped me whip up the CLI I'll admit
- bits and pieces like the windows install script and the gen-test-vectors code

I found that if I let it do any more than these fairly basic tasks I would end up spending more time undoing it's weird habits, deleting it's annoying comments and just generally rewriting stuff and being annoyed I let it do it in the first place. I even believe that by taking the boring tasks away from us we lose a level of satisfaction with having done everything ourselves and made everything the way we wanted. I am sad the time pressure pushed me to use it more than I would have liked. However it's speed increases for certain tasks are undeniable and I also feel like it may be necessary to know what it's like to use these tools, to know how not to use them. One thing do I know is that if your code has even a low level of other software depending on it this is useless, it will not write well organized well thought out code that can be built upon. And I don't like the way it does so much unnecessary stuff because it's seen other people do it with no understanding of the real purpose behind it. Anyway what are those CEO's talking about AGI? Replacing the entire workforce? Right now it's an amazing educational tool but for actually doing the work it's still mostly a gimmick. I want to work on a project I have too much pride and care to use AI on.

## Conclusion

What I learned most was about how building a proper structured codebase is a skill in itself. I probably only spent about half the time I worked on this project actually writing c code, and a very small amount of time writing the AES core code(which was all the project was originally going to be). Because I was constantly moving things around, changing little things and then testing everything again, editing configs, makefiles, running commands. Learning how to use the tools available to a programmer is a much bigger task than solving coding challenges. I'm going to spend some time now optimizing my IDE experience, learning more bash commands, learning some new concepts (networking fundamentals), reading up on C style guides and best practices.
