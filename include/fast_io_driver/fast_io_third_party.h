#pragma once
#include"fast_io_hosted.h"
//fast_io_third_party is to support 3rd-party non standard I/O libraries like MFC, QT, cppwinrt
#ifdef _AFXDLL
//MFC/ATL support
#include<afx.h>
#include"fast_io_third_party/mfc.h"
#endif


namespace fast_io
{

#ifdef _AFXDLL
using imfc_file=input_file_wrapper<mfc_file>;
using omfc_file=output_file_wrapper<mfc_file>;
using iomfc_file=io_file_wrapper<mfc_file>;

using ibuf_mfc_file = basic_ibuf<imfc_file>;
using obuf_mfc_file = basic_obuf<omfc_file>;
using iobuf_mfc_file = basic_obuf<basic_ibuf<iomfc_file>>;

using u8imfc_file=input_file_wrapper<u8mfc_file>;
using u8omfc_file=output_file_wrapper<u8mfc_file>;
using u8iomfc_file=io_file_wrapper<u8mfc_file>;

using u8ibuf_mfc_file = basic_ibuf<u8imfc_file>;
using u8obuf_mfc_file = basic_obuf<u8omfc_file>;
using u8iobuf_mfc_file = basic_obuf<basic_ibuf<u8iomfc_file>>;
#endif

}