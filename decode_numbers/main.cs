using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace decode_numbers {
    class Program {
        static void Main(string[] args){
            string line;
            var reader = new System.IO.StreamReader(args[0]);
            int current_ways = 0;
            int[] n2n1_ways = new int[]{1, 0};

            while ((line = reader.ReadLine()) != null) {
                if (line.Length == 0 || line == "\n")
                    continue;

                n2n1_ways = new int[]{1, 0};

                for (int i = 0; i < line.Length; i++) {
                    current_ways = 0;
                    var current_char = line[i];

                    if (i == 0) {
                        if (current_char == '0') {
                            System.Console.WriteLine('0');
                            break;
                        }
                        n2n1_ways[1] = 1;
                        //System.Console.WriteLine(String.Format("[{0}, {1}]", 
                        //                                        n2n1_ways[0], 
                        //                                        n2n1_ways[1]));
                        continue;
                    }
                    if ((current_char < '7' && line[i - 1] == '2') || line[i - 1] == '1')
                        current_ways = n2n1_ways[0];
                    if (current_char != '0')
                        current_ways += n2n1_ways[1];

                    if(current_ways == 0) {
                        System.Console.WriteLine('0');
                        break;
                    }
                    n2n1_ways[0] = n2n1_ways[1];
                    n2n1_ways[1] = current_ways;
                    //System.Console.WriteLine(String.Format("current char: {2}; [{0}, {1}]", 
                    //                                        n2n1_ways[0], 
                    //                                        n2n1_ways[1],
                    //                                        current_char));
                }
                System.Console.WriteLine(n2n1_ways[1]);
            } // while
        } // Main
    }
}
