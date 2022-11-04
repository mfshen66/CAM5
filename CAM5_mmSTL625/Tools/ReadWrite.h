/*	2001/11/16 nt
 *	head file of read write definitions
 */
#ifndef _ReadWrite_h_
#define _ReadWrite_h_

////////////////////////////////////////////////////////////////
//	Read and Write
int arWrite(void* ar, void* buf, int size) ;
int arRead(void* ar, void* buf, int size) ;
int arReadWrite(void* ar, int isLoad, void* buf, int size) ; // nt add 2020/4/11
////////////////////////////////////////////////////////////////

#endif