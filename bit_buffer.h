/* Damaskinskiy Konstanin */
#pragma once

#include <string>
#include <fstream>
#include "def.h"

/* Bit buffer support class */
class bit_buffer
{
public:
  static const INT BaseBufSize = 32000;
private:
  BYTE Buffer[BaseBufSize];
  INT BufSize;

  INT
    CurReadByte,         // Current byte reading
    CurReadBit;          // Current bit reading

  INT
    CurWriteBit;

public:
  /* Class constructor.
   * ARGUMENTS:
   *   - IO file names:
   *       const std::string &FileInName;
   */
  bit_buffer( const std::string &FileInName = "" ) : BufSize(0), CurReadByte(0), CurReadBit(0), CurWriteBit(0)
  {
    if (FileInName == "")
      return;
    FillBuffer(FileInName);
  } /* End of 'bit_buffer' class */

  /* Fill byte buffer from file function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (bit_buffer &) self-reference.
   */
  bit_buffer & FillBuffer( const std::string &FileName )
  {
    std::ifstream FileIn( FileName );
    if (!FileIn)
      return *this;

    FileIn.seekg(0, FileIn.end);
    BufSize = FileIn.tellg( );
    FileIn.seekg(0, FileIn.beg);

    FileIn.read((CHAR *)&Buffer, BufSize);

    return *this;
  } /* End of 'FillBuffer' function */

  /* Read bit function.
   * ARGUMENTS:
   *   - bit to read:
   *       BOOL &Bit;
   * RETURNS:
   *   (bit_buffer &) self-reference.
   */
  bit_buffer & ReadBit( CHAR &Bit )
  {
    Bit = (Buffer[CurReadByte] & (1 << CurReadBit++)) != 0;
    if (CurReadBit == 8)
    {
      CurReadByte++;
      CurReadBit = 0;
    }

    return *this;
  } /* End of 'ReadBit' function */

  /* Read bits function.
   * ARGUMENTS:
   *   - bits to read:
   *       BOOL &Bit;
   *   - number of bits to read:
   *       INT Number;
   * RETURNS:
   *   (bit_buffer &) self-reference.
   */
  bit_buffer & ReadBits( CHAR *&Bit, INT Number )
  {
    Bit = new CHAR[Number];

    for (INT i = 0; i < min(Number, BaseBufSize); i++)
      ReadBit(Bit[i]);

    return *this;
  } /* End of 'ReadBit' function */

  /* Write bit function.
   * ARGUMENTS:
   *   - bit to write:
   *       CHAR Bit;
   * RETURNS:
   *   (bit_buffer &) self-reference.
   */
  bit_buffer & WriteBit( CHAR Bit )
  {
    if (CurWriteBit != 0)
      Buffer[BufSize - 1] += Bit << CurWriteBit++;
    else
      Buffer[BufSize++] += Bit << CurWriteBit++;

    if (CurWriteBit == 8)
      CurWriteBit = 0;

    return *this;
  } /* End of 'WriteBit' function */

   /* Write bits function.
   * ARGUMENTS:
   *   - bits array to write:
   *       CHAR *Bit;
   *   - number of bits to write:
   *       INT Number;
   * RETURNS:
   *   (bit_buffer &) self-reference.
   */
  bit_buffer & WriteBit( CHAR *Bit, INT Number )
  {
    for (INT i = 0, sz = min(Number, BaseBufSize - BufSize); i < sz; i++)
      WriteBit(Number);

    return *this;
  } /* End of 'WriteBit' function */
}; /* End of 'bit_buffer' class */

/* END OF 'bit_buffer.h' FILE */
