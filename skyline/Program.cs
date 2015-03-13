using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;

namespace ss {
  class building {
    public int l, h, r;
    public building(int l_, int h_, int r_) {
       h = h_; r = r_; l = l_;
    }
    public building (building other) {
      this.l = other.l; this.h = other.h; this.r = other.r;
    }
  }

  class skyline {
    public skyline() {
      inProcess_ = new Dictionary<int,List<building>>();
      skyline_ = new List<int>();
    }

    private Dictionary<int, List<building>> inProcess_;
    private List<int> skyline_;

    public void reset() {
      inProcess_.Clear();
      skyline_.Clear();
    }

    static building findHighest(List<building> lb) {
      var maxH = lb.Max(x => x.h);
      return lb.Where(x => x.h == maxH).First();
    }
    public void print() {
      for(int i = 0; i < skyline_.Count; ++i) {
        Console.Write(skyline_[i]);
        if(i != skyline_.Count - 1)
          Console.Write(' ');
      }
      Console.Write('\n');
    }

    public void processPoint(int point, List<building> lb) {
      var be = allBuildingsEndingHere(point, lb);
      var bs = allBuildingsStartingHere(point, lb);
      building highestInProcess = null;

      // remove all that end here
      if(be.Count != 0) {
        var hbe = findHighest(be);
        List<int> keysToRemove = new List<int>();
        foreach(var p in inProcess_) {
          if(p.Value != null) {
            p.Value.RemoveAll(b => be.Exists(b1 => b1 == b));
          }
          if(p.Value.Count == 0) {
            keysToRemove.Add(p.Key);
          }
        }
        for(int i = 0; i < keysToRemove.Count; ++i) {
          inProcess_.Remove(keysToRemove[i]);
        }

        highestInProcess = null;
        foreach(var p in inProcess_) {
          if(p.Value != null) {
            var tmp = findHighest(p.Value);
            if(highestInProcess == null || highestInProcess.h < tmp.h) {
              highestInProcess = new building(tmp);
            }
          }
        }

        if(highestInProcess != null && hbe.h > highestInProcess.h) {
          if(bs.Count > 0) {
            var hbs = findHighest(bs);
            if(highestInProcess.h >= hbs.h) {
              skyline_.Add(hbe.r);
              skyline_.Add(highestInProcess.h);
            }
          }
          else {
            skyline_.Add(hbe.r);
            skyline_.Add(highestInProcess.h);
          }
        }
        else if(highestInProcess == null) {
          if(bs.Count == 0) {
            skyline_.Add(hbe.r);
            skyline_.Add(0);
          }
        }
      }
      
      highestInProcess = null;
      foreach(var p in inProcess_) {
        if(p.Value != null) {
          var tmp = findHighest(p.Value);
          if(highestInProcess == null || highestInProcess.h < tmp.h) {
            highestInProcess = new building(tmp);
          }
        }
      }

      // insert new buildings
      if(bs.Count != 0) {
        inProcess_[point] = bs;
        var hbs = findHighest(bs);
        
        if(highestInProcess == null || hbs.h > highestInProcess.h) {
          highestInProcess = new building(hbs);
          if(be.Count != 0) {
            var hbe = findHighest(be);
            if(hbs.h != hbe.h) {
              skyline_.Add(hbs.l);
              skyline_.Add(hbs.h);
            }
          }
          else {
            skyline_.Add(hbs.l);
            skyline_.Add(hbs.h);
          }
        }
      }
    }
    static List<building> allBuildingsStartingHere(int point, 
                                                   List<building> buildings) {
      List<building> result = new List<building>();
      for(int i = 0; i < buildings.Count; ++i) {
        if(buildings[i].l == point) {
          result.Add(buildings[i]);
        }
      }
      return result;
    }

    static List<building> allBuildingsEndingHere(int point, 
                                                 List<building> buildings) {
      List<building> result = new List<building>();
      for(int i = 0; i < buildings.Count; ++i) {
        if(buildings[i].r == point) {
          result.Add(buildings[i]);
        }
      }
      return result;
    }
  }
  class Program {
    static readonly Regex bre = new Regex(".*\\((\\d+),(\\d+),(\\d+)\\).*");
    
    static void process(string s) {
      var bstrs = s.Split(';');
      var buildings = new List<building>(bstrs.Length);
      var points = new List<int>(bstrs.Length);
      skyline sline = new skyline();

      for(int i = 0; i < bstrs.Length; ++i) {
        Match m = bre.Match(bstrs[i]);
        if(m.Success) {
          buildings.Add(new building(Convert.ToInt32(m.Groups[1].Value),
                                     Convert.ToInt32(m.Groups[2].Value),
                                     Convert.ToInt32(m.Groups[3].Value)));
          int p = Convert.ToInt32(m.Groups[1].Value);
          if(!points.Exists(x => x == p)) {
            points.Add(p);
          }
          
          p = Convert.ToInt32(m.Groups[3].Value);

          if(!points.Exists(x => x == p)) {
            points.Add(p);
          }
        }
        else {
          Console.WriteLine("regex failed", bstrs[i]);
        }
      }
      points.Sort();

      for(int i= 0; i < points.Count; ++i) {
        sline.processPoint(points[i], buildings);
      }
      sline.print();
    }

    static void processFile(string fname) {
      using(var sr = new StreamReader(fname)) {
        string line;
        while((line = sr.ReadLine()) != null) {
          process(line);
        }
      }
    }

    static void Main(string[] args) {
      if(args.Length < 1) {
        Console.WriteLine("Usage prgrm <file>");
        return;
      }
      var sw = new Stopwatch();
      sw.Start();
      processFile(args[0]);
      sw.Stop();
      var ts = sw.Elapsed;
      Console.WriteLine(String.Format("Elapsed: {0}.{1:00}s", ts.Seconds, ts.Milliseconds/10));
    }
  }
}
