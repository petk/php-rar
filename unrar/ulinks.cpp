#include "rar.hpp"



int ExtractLink(ComprDataIO &DataIO,Archive &Arc,char *DestName,uint &LinkCRC,bool Create)
{
#if defined(SAVE_LINKS) && defined(_UNIX)
  char FileName[NM];
  if (IsLink(Arc.NewLhd.FileAttr))
  {
    int DataSize=Min(Arc.NewLhd.PackSize,sizeof(FileName)-1);
    DataIO.UnpRead((byte *)FileName,DataSize);
    FileName[DataSize]=0;
    if (Create)
    {
      CreatePath(DestName,NULL,true);
      if (symlink(FileName,DestName)==-1)
        if (errno==EEXIST)
          Log(Arc.FileName,St(MSymLinkExists),DestName);
        else
        {
          Log(Arc.FileName,St(MErrCreateLnk),DestName);
          ErrHandler.SetErrorCode(RAR_WARNING);
        }
    }
    LinkCRC=CRC(0xffffffff,FileName,DataSize);
    return(1);
  }
#endif
  return(0);
}