### Introduction

This is a naive personal implimentation of the AES encription algorithm in c. It has not been sufficiently vetted for security issues to be used for any sensitive application. It has been created for personal educational purposes only. The source of the specifications for building this algorithm was the official FIPS 197 document published in 2001 (and updated 2023) by the National Institute of Standards and Technology. This is an excelent resource for a project like this, as it has both detailed explanation and specifications, and examples that were used for testing. Terminology and names have mostly been pulled directly from this document and comments will frequently refer to specific sections in order to explain the code. The URL to the version used is

https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf

###  Personal AI Policy
No code in the core directory has been written by AI and AI autocomplete was dissabled, this was to make sure that everything was intentional. AI was used to create test code, by giving it specific test vectors to format into tests. The logic of this was that since this is code that since this is code that only runs once to verify everything is working, it is less critical that it is meticulously thought through or well structured. However, upon reflection it became obvious that the AI code was not up to sufficient standard, and time was spent rewriting it, finding sources for the data, and debugging it when it made false assumptions about how the program worked. 

### Mode of operation

https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38a.pdf

### Functionality
