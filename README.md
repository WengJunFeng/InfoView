# InfoView

This is a Microsoft Windows utility/tool I wrote back in the days when C++ and VC++ were hot. The front-end is built using VC++/MFC, and there is a DLL that the app uses, and it is written in C language. The C library uses Windows SDK (which is now upgraded to WINSDK version 10), and the rest remains in Visual C++/MFC.

## What does this program do?

This program is like a Task Manager/Process Manager on steroids. With this, you have the ability to navigate to any control in any Windows app that has a UI, and view/change the how it behaves. You can enable/disable buttons, make them readonly, hide/unhide controls, and even force programs to close... and more!

## Why did I write this program?
When I moved from 16 bit Windows Operating System to a 32 bit Windows Operating System, things have changed! I used to be able to share clipboard across instances of applications because all the applications shared common memory space. With the Windows 32-bit Operating System, things have changed, and I no longer have shared memory anymore, and the inter-process communication has become harder for all the right reasons.

I wanted to write a program, where I can still have some sort of communication across processes, where I can retrieve the value of a text box from another process space. The only way I could do it is by injecting a Dynamically Linked Library (DLL) into other process memory, and establish inter-process communication using that common DLL. Most of the password stealer programs, keyboard hijacking programs do this exact thing! And yes, you can view passwords of another process using this application, and even capture keyboard messages too.

The code comes with no warranty, use it at your risk! This was written when my understanding of C++ was very weak, and if you find problems, I am not planning on updating or changing it.
