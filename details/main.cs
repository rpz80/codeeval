using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace mrs {
  class Program {
    static int FindMRS(List<int> l, int days) {
      int ret = 0;
      int tmp = 0;

      for(int i = 0; i < l.Count - days + 1; ++i) {
        for(int j = i; j < days + i; ++j) {
          tmp += l[j];
        }
        if(tmp > ret)
          ret = tmp;
        tmp = 0;
      }
      return ret > 0 ? ret : 0;
    }

    static void test() {
      var t1 = new List<int>{7, -3, -10, 4, 2, 8, -2, 4, -5, -2};
      Console.WriteLine(FindMRS(t1, 5));

      t1 = new List<int>{-4, 3, -10, 5, 3, -7, -3, 7, -6, 3};
      Console.WriteLine(FindMRS(t1, 6));

      t1 = new List<int>{-7, 0, -45, 34, -24, 7};
      Console.WriteLine(FindMRS(t1, 3));
    }

    static void Main(string[] args) {
      //test();
      using(var sr = new StreamReader(args[0])) {
        string line;
        List<int> l;
        int days;

        while((line = sr.ReadLine()) != null) {
          var splts = line.Split(';');
          days = Convert.ToInt32(splts[0]);
          var splts2 = splts[1].Split(' ');
          l = splts2.Select(x => Convert.ToInt32(x)).ToList();
          
          Console.WriteLine(FindMRS(l, days));
        }
      }
    }
  }
}
