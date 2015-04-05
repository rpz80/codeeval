using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace jt {
  class Program {
    static readonly int MAX_LEN = 80;
    static List<List<string>> char80ArrayFromString(string s) {
      var words = s.Split(' ');
      List<List<string>> result = new List<List<string>>();
      List<string> tmp = new List<string>();
      int tmpLen = 0;
      for(int i = 0; i < words.Length; ++i) {
        if(words[i].Length + tmpLen - 1 >= MAX_LEN) {
          result.Add(new List<string>(tmp));
          tmp.Clear();
          tmpLen = 0;
        }
        tmp.Add(words[i]);
        tmpLen += words[i].Length + 1;
      }
      if(tmp.Count > 0)
        result.Add(tmp);
      return result;
    }

    static int spacesToAdd(List<string> l) {
      int wordLen = 0;
      for(int i = 0; i < l.Count; ++i) {
        wordLen += l[i].Length;
      }
      return MAX_LEN - (wordLen + l.Count - 1);
    }

    static List<string> makeSpaceList(int wordCount, int spaceCount) {
      List<string> result = new List<string>(wordCount - 1);
      for(int i = 0; i < wordCount - 1; ++i) {
        result.Add(" ");
      }
      int index = 0;
      while(spaceCount > 0) {
        result[index] += ' ';
        --spaceCount;
        if(index == result.Count - 1) {
          index = 0;
        }
        else {
          ++index;
        }
      }
      return result;
    }

    static string mergeWordsAndSpaces(List<string> words, List<string> spaces) {
      StringBuilder result = new StringBuilder();
      for(int i = 0; i < words.Count-1; ++i) {
        result.Append(words[i]);
        result.Append(spaces[i]);
      }
      result.Append(words[words.Count - 1]);
      return result.ToString();
    }

    static void process(string s) {
      var c80List = char80ArrayFromString(s);
      for(int i = 0; i < c80List.Count - 1; ++i) {
        int sta = spacesToAdd(c80List[i]);
        var sl = makeSpaceList(c80List[i].Count, sta);
        Console.WriteLine(mergeWordsAndSpaces(c80List[i], sl));
      }
      Console.WriteLine(String.Join(" ",c80List[c80List.Count - 1]));
    }

    static void Main(string[] args) {
      using(var sr = new StreamReader(args[0])) {
        string line;
        while((line = sr.ReadLine()) != null) {
          process(line);
        }
      }
    }
  }
}
