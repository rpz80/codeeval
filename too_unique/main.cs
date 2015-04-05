using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace tu {
  struct Rect {
    public int tlRow, tlCol, brRow, brCol;
    public int Width {
      get {
        return brCol - tlCol;
      }
    }

    public int Height {
      get {
        return brRow - tlRow;
      }
    }
  }

  enum GrowState {
    CanGrow,
    Wall,
    NotUnique
  }

  class RoamingRect {
    private Field f;
    private Rect rect;    
    private List<char> inProcess;

    public int Width {
      get {
        return rect.Width;
      }
    }

    public int Height {
      get {
        return rect.Height;
      }
    }

    public RoamingRect(Field f) : this(f, 0, 0) {
    }

    public RoamingRect(Field f, int startRow, int startCol) {
      this.f = f;
      rect.tlRow = startRow;
      rect.tlCol = startCol;
      rect.brRow = startRow + 1;
      rect.brCol = startCol + 1;
      inProcess = new List<char>();
      inProcess.Add(f.at(startRow, startCol));
    }

    public RoamingRect(RoamingRect other) {
      this.f = other.f;
      this.rect = other.rect;
      this.inProcess = new List<char>(other.inProcess);
    }

    public RoamingRect(string s) : this(new Field(s)) {
    }

    public GrowState CanGrowRight() {
      if(rect.brCol == f.n) {
        return GrowState.Wall;
      }
      var tmpCol = rect.brCol + 1;
      var tmpList = new List<char>();
      for(int i = rect.tlRow; i < rect.brRow; ++i) {
        for(int j = rect.brCol; j < tmpCol; ++j) {
          if(inProcess.Exists(x => x == f.at(i,j))) {
            return GrowState.NotUnique;
          }
          if(tmpList.Exists(x => x == f.at(i,j))) {
            return GrowState.NotUnique;
          }
          tmpList.Add(f.at(i,j));
        }
      }
      return GrowState.CanGrow;
    }

    public GrowState CanGrowBottom() {
      if(rect.brRow == f.m) {
        return GrowState.Wall;
      }
      var tmpList = new List<char>();
      for(int j = rect.tlCol; j < rect.brCol; ++j) {
        if(inProcess.Exists(x => x == f.at(rect.brRow,j))) {
          return GrowState.NotUnique;
        }
        if(tmpList.Exists(x => x == f.at(rect.brRow,j))) {
          return GrowState.NotUnique;
        }
        tmpList.Add(f.at(rect.brRow,j));
      }
      return GrowState.CanGrow;
    }

    public Rect GrowBottom() {
      for(int j = rect.tlCol; j < rect.brCol; ++j) {
        inProcess.Add(f.at(rect.brRow,j));
      }
      ++rect.brRow;
      return rect;
    }

    public Rect GrowRight() {
      rect.brCol += 1;
      for(int i = rect.tlRow; i < rect.brRow; ++i) {
        for(int j = rect.brCol - 1; j < rect.brCol; ++j) {
          inProcess.Add(f.at(i, j));
        }
      }
      return rect;
    }

    public Rect shrinkLeft() {
      for(int i = rect.tlRow; i < rect.brRow; ++i) {
        inProcess.RemoveAll(x => x == f.at(i, rect.tlCol));
      }
      rect.tlCol += 1;
      return rect;
    }

    public Rect shrinkTop() {
      for(int i = rect.tlCol; i < rect.brCol; ++i) {
        inProcess.RemoveAll(x => x == f.at(rect.tlRow, i));
      }
      rect.tlRow += 1;
      return rect;
    }

    public Rect getRect() {
      return rect;
    }

    public void Print() {
      for(int i = rect.tlRow; i < rect.brRow; ++i) {
        for(int j = rect.tlCol; j < rect.brCol; ++j) {
          Console.Write(string.Format("{0, -2}", f.at(i, j)));
        }
        Console.WriteLine();
      }
    }
  }

  class Field {
    private List<char> field_;
    private int m_, n_;

    public int m {
      get {
        return m_;
      }
    }

    public int n {
      get {
        return n_;
      }
    }

    public void Set(int i, int j, char c) {
      field_[i*n_ + j] = c;
    }

    public char at(int i, int j) {
      if(i < 0 || i >= m_ || j < 0 || j >= n_) {
        throw new Exception("'at' out of range");
      }
      return field_[i*n_ + j];
    }
    public Field(string s) {
      for(int i = 0; i < s.Length; ++i) {
        if(s[i] == '\n' || s[i] == '\r') {
          break;
        }
        ++n_;
      }
      field_ = s.Where(c => c != '\n' && 
                            c != '\r' && 
                            c != ' ')
                .ToList();
      m_ = field_.Count / n_;
    }

    public void Print() {
      for(int i = 0; i < m_; ++i) {
        for(int j = 0; j < n_; ++j) {
          Console.Write(field_[i*n_ + j]);
        }
        Console.WriteLine();
      }
    }
  }

  class Program {
    static void Process(string s) {
      Field f = new Field(s);
      var result = new List<RoamingRect>();

      for(int i = 0; i < f.m; ++i) {
        for(int j = 0; j < f.n; ++j) {
          RoamingRect rr = new RoamingRect(f, i, j);
          while(true) {
            RoamingRect copyRR = new RoamingRect(rr);
            while(true) {
              if(result.Count == 0) {
                result.Add(new RoamingRect(copyRR));
              }
              else if(result[0].Width*result[0].Height < copyRR.Width*copyRR.Height) {
                result.Clear();
                result.Add(new RoamingRect(copyRR));
              }
              else if(result[0].Width*result[0].Height == copyRR.Width*copyRR.Height) {
                result.Add(new RoamingRect(copyRR));
              }

              if(copyRR.CanGrowBottom() != GrowState.CanGrow)
                break;
              copyRR.GrowBottom();
            }
            if(rr.CanGrowRight() != GrowState.CanGrow)
              break;
            rr.GrowRight();
          }
        }
      }

      for(int i = 0; i < result.Count; ++i) {
        for(int ii = result[i].getRect().tlRow; 
            ii < result[i].getRect().brRow;
            ++ii) {
          for(int jj = result[i].getRect().tlCol; 
              jj < result[i].getRect().brCol;
              ++jj) {
            f.Set(ii, jj, '*');
          }
        }
      }
      f.Print();
    }

    static void Test() {
      var testField = @"rzqicaiiaege
                        ccwnulljybtu
                        jxtxupauwuah
                        oqikzgqrzpdq
                        vblalwdjbdwn
                        ahjeencuclbo";
      // Field tests
      Field f = new Field(testField);
      Debug.Assert(f.m == 6 && f.n == 12, 
                   String.Format("f.m == 6 && f.n == 12 failed with m = {0} and n= {1}",
                                 f.m, f.n));

      Debug.Assert(f.at(0, 0) == 'r', 
                   String.Format("f.at(0, 0) == 'r' failed with f.at(0, 0) == {0}",
                                 f.at(0, 0)));

      Debug.Assert(f.at(4, 10) == 'w', 
                   String.Format("f.at(4, 10) == 'w' failed with f.at(4, 10) == {0}",
                                 f.at(4, 10)));

      // Roaming tests
      RoamingRect rr = new RoamingRect(testField);
      Debug.Assert(rr.CanGrowRight() == GrowState.CanGrow, 
                   String.Format("rr.CanGrowRight() == GrowState.CanGrow failed with rr.CanGrowRight() == {0}",
                                  rr.CanGrowRight()));
      rr.GrowRight();

      Debug.Assert(rr.CanGrowBottom() == GrowState.NotUnique, 
                   String.Format("rr.CanGrowBottom() == GrowState.NotUnique failed with rr.CanGrowBottom() == {0}",
                                  rr.CanGrowBottom()));
      rr.GrowBottom();
      rr.shrinkTop();

      Process(testField);
    }
    static void Main(string[] args) {
      using (var sr = new StreamReader(args[0])) {
        Process(sr.ReadToEnd());
      }
      //Test();
    }
  }
}
