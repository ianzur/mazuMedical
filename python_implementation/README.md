# re-implementation 

@ianzur: (Dec 2021) python implementation is a work in progress

### install
This is also a painful install.

Just like with the c++ implementation, you will have to both build qt from source. But with the additional step of generating the [PySide6](https://doc.qt.io/qtforpython/gettingstarted-linux.html) bindings.

After building replace the use `python setup.py bdist_wheel ...` to create the a wheel you can install for in a separate virtual environment for this project.

`cringbuffer`: an implementation of a ring buffer that is slightly modified for this project.

@ianzur: This project was my first attempt at making an application. 4 years later I am re-implementing the project in python with several improvements.