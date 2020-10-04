#pragma once

//fast_io_legacy.h deals with legacy C <stdio.h> and C++ <iostream>/<fstream>/<sstream> interface

#include<streambuf>
#include<fstream>
#include<sstream>
#include<istream>
#include<ostream>

#include"fast_io_hosted.h"
#include"fast_io.h"
#include"fast_io_legacy_impl/c/impl.h"
#include"fast_io_legacy_impl/cpp/streambuf_io_observer.h"
#include"fast_io_legacy_impl/cpp/filebuf_file.h"
