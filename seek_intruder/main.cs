using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Threading;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace si {
  class IpSearcher {
    public enum IpType {
      BIN     = 0,
      BIN_DOT = 1,
      OCT     = 2,
      DEC     = 3,
      DEC_DOT = 4,
      HEX     = 5,
      HEX_DOT = 6,
      OCT2    = 7
    }

    public IpSearcher() {
      ips_ = new Dictionary<UInt32, int>();
      ip_re_ = new List<Regex>(4);
      ip_re_.Add(new Regex(@"([10]{31,33})"));
      ip_re_.Add(new Regex(@"([10]{4,5}\.[10]{4,5}\.[10]{4,5}\.[10]{4,5})"));
      ip_re_.Add(new Regex(@"(0\d{1,4}\.0\d{1,4}\.0\d{1,4}\.0\d{1,4})"));
      ip_re_.Add(new Regex(@"([0-9]{11,12})"));
      ip_re_.Add(new Regex(@"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})"));
      ip_re_.Add(new Regex(@"(0x[abcdef0-9]{8})"));
      ip_re_.Add(new Regex(@"(0x[abcdef0-9]{2}.0x[abcdef0-9]{2}.0x[abcdef0-9]{2}.0x[abcdef0-9]{2})"));
      ip_re_.Add(new Regex(@"(0[0-9]{11})"));
    }

    public UInt32 IpToUint(string str, int b) {
      UInt32 ip = 0;
      var splts = str.Split('.');
      try {
        var f = Convert.ToUInt32(splts[0], b);
        var s = Convert.ToUInt32(splts[1], b);
        var t = Convert.ToUInt32(splts[2], b);
        var r = Convert.ToUInt32(splts[3], b);
        ip = ip | r | (t << 8) | (s << 16) | (f << 24);
        return ip;
      }
      catch (Exception) {
        return 0;
      }
    }

    public string IpToString(UInt32 n) {
      return ((n >> 24) & 0xff).ToString() + "." +
             ((n >> 16) & 0xff).ToString() + "." +
             ((n >> 8) & 0xff).ToString()  + "." +
              (n & 0xff).ToString();
    }

    void ConvertAndAdd(MatchCollection m, IpType type) {
      UInt32 ip = 0;
      bool parsed = true;
      foreach(var x in m) {
        parsed = true;
        string str = x.ToString();
        switch (type) {
          case IpType.BIN:
            try {
              ip = Convert.ToUInt32(str, 2);
            } catch {
              parsed = false;
            }
            break;
          case IpType.BIN_DOT:
            ip = IpToUint(str, 2);
            break;
          case IpType.OCT:
            ip = IpToUint(str, 8);
            break;
          case IpType.DEC:
            parsed = UInt32.TryParse(str, out ip);
            break;
          case IpType.DEC_DOT:
            ip = IpToUint(str, 10);
            break;
          case IpType.HEX:
            try {
              ip = Convert.ToUInt32(str, 16);
            } catch {
              parsed = false;
            }
            break;
          case IpType.HEX_DOT:
            ip = IpToUint(str, 16);
            break;
          case IpType.OCT2:
            try {
              ip = Convert.ToUInt32(str, 8);
            } catch {
              parsed = false;
            }
            break;
        }
        if(parsed && ip !=0 && CheckIp(IpToString(ip))) {
          if(ips_.ContainsKey(ip))
            ips_[ip]++;
          else
            ips_.Add(ip, 1);
        }
      }
    }

    private bool CheckIp(string s) {
      var splts = s.Split('.');
      foreach(var sp in splts) {
        try {
        UInt32 i = Convert.ToUInt32(sp, 10);
        if(i > 255)
          return false;
        }
        catch(Exception) {
          return false;
        }
      }
      return true;
    }

    public void Parse(string s) {
      for(int i = 0; i < ip_re_.Count; ++i) {
        MatchCollection matches = ip_re_[i].Matches(s);
        if(matches.Count != 0) {
          ConvertAndAdd(matches, (IpType)i);
        }
      }
    }

    public void Print() {
      foreach(var p in ips_) {
        Console.WriteLine(String.Format("ip[{0}] = {1}", p.Key, p.Value));
      }
    }

    public string MostWanted() {
      return IpToString(ips_.FirstOrDefault(x => x.Value == ips_.Values.Max()).Key);
    }

    private Dictionary<UInt32, int> ips_;
    private List<Regex> ip_re_;
  }

  class Program {
    static void Main(string[] args) {
      Stopwatch stopWatch = new Stopwatch();
      stopWatch.Start();
      if (args.Length != 1) {
        Console.WriteLine("Usage: si <file-name>");
        return;
      }
      try {
        using (var sr = new StreamReader(args[0])) {
          string s;
          IpSearcher ips = new IpSearcher();
          while((s = sr.ReadLine()) != null) {
            ips.Parse(s);
          }
          Console.WriteLine(ips.MostWanted());
        }
      }
      catch (Exception e) {
        Console.WriteLine(String.Format("Something gone wrong... {0}", e));
        return;
      }
      stopWatch.Stop();
      Console.WriteLine(String.Format("Elapsed: {0}", stopWatch.Elapsed));
    }

  }
}
