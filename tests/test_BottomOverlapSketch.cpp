#include "common.hpp"
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include "../include/BottomOverlapSketch.hpp"
#include "../include/OverlapInfo.hpp"
#include "../include/Sequence.hpp"

class BottomOverlapSketchTest : public BottomOverlapSketch {
    
};

BOOST_AUTO_TEST_SUITE(TestBottomOverlapSketch)

BOOST_AUTO_TEST_CASE(TestSelfComparison) {
    const std::string s("TTGGTATGCTTCGTTCAGTTTCAGGTTTGGGTGTTTAACCGTTTTTTCACATTT"
                        "ATCGTGAAACGCTTTCGCGTTTTCGTGCCGCTTCAGCTCTGGAAATCGACCTGA"
                        "CGGGGTCGGTGGAGAACGATTATTTTTCTGTAATGAGCAGAACGAAAAGGTGGT"
                        "GGTCACTGGCAGGGGCGACAGTATCCAGCCGTATCCCATTCCAGGAGTTTTTGA"
                        "ACTGAATGGCAAAGGCACCAGTACGCGCCCACGCTGACGGTTTCTAACCTGTAC"
                        "GGTATGGTCACCCGGGATGGCGGAAGATATGCGAAGTCTGGTCAGCCGGAACGG"
                        "TCCGGCGTAGGTTCGCCGTTTTCTGGATGCGGTGAACCGTCAGCGAAACAGTTA"
                        "CGCCGTCCGGAGCAGGGTGATCAGCCGCTGGCGCACTGAGCAGTGCAGCAGACG"
                        "AGCGCGGTGATTACCTCACTTTGCCAGTCCCGCCGACGGAAACGGATGGCGCTG"
                        "TTTTCAGGACGTCATGCTGGCCAGCACCTGCACC");

    BottomOverlapSketch bSketch(s, 12, 1536, false);
    auto overlapInfo = bSketch.getOverlapInfo(bSketch, 0.2);

    BOOST_CHECK_EQUAL(overlapInfo.score, 1.0);
    BOOST_CHECK_EQUAL(overlapInfo.a1, 0);
    BOOST_CHECK_EQUAL(overlapInfo.a2, 509);
    BOOST_CHECK_EQUAL(overlapInfo.b1, 0);
    BOOST_CHECK_EQUAL(overlapInfo.b2, 509);
}

BOOST_AUTO_TEST_CASE(TestSelfReverseComplementaryComparison) {
    const std::string s1("TTGGTATGCTTCGTTCAGTTTCAGGTTTGGGTGTTTAACCGTTTTTTCACATTT"
                         "ATCGTGAAACGCTTTCGCGTTTTCGTGCCGCTTCAGCTCTGGAAATCGACCTGA"
                         "CGGGGTCGGTGGAGAACGATTATTTTTCTGTAATGAGCAGAACGAAAAGGTGGT"
                         "GGTCACTGGCAGGGGCGACAGTATCCAGCCGTATCCCATTCCAGGAGTTTTTGA"
                         "ACTGAATGGCAAAGGCACCAGTACGCGCCCACGCTGACGGTTTCTAACCTGTAC"
                         "GGTATGGTCACCCGGGATGGCGGAAGATATGCGAAGTCTGGTCAGCCGGAACGG"
                         "TCCGGCGTAGGTTCGCCGTTTTCTGGATGCGGTGAACCGTCAGCGAAACAGTTA"
                         "CGCCGTCCGGAGCAGGGTGATCAGCCGCTGGCGCACTGAGCAGTGCAGCAGACG"
                         "AGCGCGGTGATTACCTCACTTTGCCAGTCCCGCCGACGGAAACGGATGGCGCTG"
                         "TTTTCAGGACGTCATGCTGGCCAGCACCTGCACC");

    const std::string s2(Sequence::revCompSeq(s1));

    BottomOverlapSketch b1(s1, 12, 1536, false);
    BottomOverlapSketch b2(s2, 12, 1536, false);

    auto overlapInfo = b1.getOverlapInfo(b2, 0.2);

    BOOST_CHECK_EQUAL(overlapInfo.score, 0);
    BOOST_CHECK_EQUAL(overlapInfo.a1, 0);
    BOOST_CHECK_EQUAL(overlapInfo.a2, 0);
    BOOST_CHECK_EQUAL(overlapInfo.b1, 0);
    BOOST_CHECK_EQUAL(overlapInfo.b2, 0);
}

//depends on hashing function??

BOOST_AUTO_TEST_CASE(TestComparison) {
    const std::string s1("TTGGTATGCTTCGTTCAGTTTCAGGTTTGGGTGTTTAACCGTTTTTTCACATTT"
                         "ATCGTGAAACGCTTTCGCGTTTTCGTGCCGCTTCAGCTCTGGAAATCGACCTGA"
                         "CGGGGTCGGTGGAGAACGATTATTTTTCTGTAATGAGCAGAACGAAAAGGTGGT"
                         "GGTCACTGGCAGGGGCGACAGTATCCAGCCGTATCCCATTCCAGGAGTTTTTGA"
                         "ACTGAATGGCAAAGGCACCAGTACGCGCCCACGCTGACGGTTTCTAACCTGTAC"
                         "GGTATGGTCACCCGGGATGGCGGAAGATATGCGAAGTCTGGTCAGCCGGAACGG"
                         "TCCGGCGTAGGTTCGCCGTTTTCTGGATGCGGTGAACCGTCAGCGAAACAGTTA"
                         "CGCCGTCCGGAGCAGGGTGATCAGCCGCTGGCGCACTGAGCAGTGCAGCAGACG"
                         "AGCGCGGTGATTACCTCACTTTGCCAGTCCCGCCGACGGAAACGGATGGCGCTG"
                         "TTTTCAGGACGTCATGCTGGCCAGCACCTGCACC");

    const std::string s2("TCGTGTACTTCGTTCGGTTATCAATTTGGGTGTTTAACCGTTTCGCATTTATCG"
                         "TGAAACGCTTTCGCGTTTTCGTGCGCCGCTTCAAAGAGACGCGAAACAGCAGTC"
                         "AAACTTAAATTTCTGCTCATTTGCCGCCAGCAGTTGGGCGGTTAATGTATCGAC"
                         "ATAAAATCCCGTAAAAAAGCCCGCACAGGCGGCCTTTAGTGATGAAAGGGTGCG"
                         "TTAAACGATGCTGATTGCCGTTCCGGCAAACGCGGTCCGTTTTTTCGTCTCGTC"
                         "GCTGGCGGCCTCCGGCCGAGCACATCCTCATAACGGAACGTGCCGGACTTGTAG"
                         "AACGTCAGCGTGGTGCTGGTCTGGTCAGCAGCAACCATATGCCAACAGCAGCAC"
                         "CGTCAGTGGTGCCATCCCACACAACCAACTTACGGCTGGAGGTGTCCAGCATCA"
                         "GCAGGGGTCGTGCAGAGCGCTTTCACTCAATCCGCCGGGCGCGGTTGCGGTATG"
                         "AGCTCAGGTCACTGTTGCCCTGCGGCTGGTAATGGGTAAAGGTTTCTTTGCTCG"
                         "TCATAAACATCCTTACACTGGTGTGTTCAGCAAATCGTTAACGCGGCATCAGAT"
                         "GCCGGTTACCTCGGCCAGCGGTGCCGGTTGCCCCTGCATCGAACGATCAGCGCG"
                         "GTGTCACTGCGCGCCTGTGCCTCGTGGTGCTGCGGCCAGAATGCGGCGGAGCCC"
                         "GTTTTTCACGGTCGCCGGGGGGTTTTCTGCCAGCACGCGTGCCTGTTCTTCGCG"
                         "TCCGTGAGCCTCCCGCA");

    BottomOverlapSketch b1(s1, 12, 1536, false);
    BottomOverlapSketch b2(s2, 12, 1536, false);

    auto overlapInfo = b1.getOverlapInfo(b2, 0.2);

    BOOST_CHECK_EQUAL(overlapInfo.score, 0.9603485527371235);
    BOOST_CHECK_EQUAL(overlapInfo.a1, 23);
    BOOST_CHECK_EQUAL(overlapInfo.a2, 72);
    BOOST_CHECK_EQUAL(overlapInfo.b1, 23);
    BOOST_CHECK_EQUAL(overlapInfo.b2, 67);
}

BOOST_AUTO_TEST_CASE(TestComparison2) {
    const std::string s1("TTGATGCTTCGTTCAGTATCAGTTGGGTGTTTAACCGTTTTCGCGTTATCGTGAAACGCTTTCGCGTTTTTCGTGCCGCTTCAGCCAGCATCTGCGCCACACCACCGAGCACCATACTGGCACCGAGAAAACAGGATGCCGGTCGCCCGCCACCGGCCCCAATGGCTGCCCCCCATGCTGGGAGCTCCGGCGGTAAAGAATGATCCGGCAATATGGCGGCAGCCCGGGACAATCTGGAATACGCCACCTGGCTTGGCCCCGGCGACTCTGGGAACAATATGAATTACAGCGCCATCAGGCGAGTCTCATGTAACTGCGCCGTTAACCGGACGTGCTGACGTCCCGCCGGCAATCCGTACCTGATACCAGCCGTCCTTTCAGTTTCTGACGAAAACGCCGGGGAGCTGTGTGGCCAGTGCCGGATAGCTTCAGCCCCCGTTTTCACACGAAGGTCGATGCGGCGACCAAATCGGTGTAGAAATCCCCGTAAAAGGCAGATGCGCGCCATGCCGGTGACCAGAGGGTGATGTGTGCGTCGCTGCCATTTGTCGGTGTACCTCTCGTTTTGCTCGGTTGTTCGGGGAATATGGTGCAGCAGCTCGCCGTCGCCACCAGTAAATTGCGGCGTGATTCGGCACTGATGAACCAAAACAGCACAACTTTATCGCCCGGCTGTGCCGCTGACAGCAACTTTTCGATGCAGCCGTCGCCTCCGTTATCCAGATGCAGATTCAGCGTTACGCCACCAGTCATCCTCACGATGAAAATTCCGGCATCTCAATCCCCGCCAGATGATGGCATCCGGAACAGTGTGTAACAATGATCCGTCACCGTGCTCAAGCGCCGCCCGGTGAGATGCGGCACAGCGAACATGGAATCTTCCCAGCAGACCAGCCACCGGCAATCCTCTGCTGCCTGCAGCCCGCCGGTCGGCCTCACTCAGCCAGACGGAATACCGGGGTGGCTGTGGACCAACTGCCTAATCTCACCTGCATTTCTGCCGCAGCCAGTCTTCCGGCGACATGCGAATGCGCCTCGGCTCGCCGGAGAAATATTCGCAGAATATCGCCACACTCCGGCGTGCTTACCACGAAACCGCACGACTCCATGGCGCACATCGCCGGGCGTGCGCGCCAGAATCGCTGATTCTGTCTGGCTGTCATGGGATTTACCTGCGAAGTTTGTTATTAGGAAAGGAAACCGCCAAGTTGCCGACGTTATTGCGGAACTTACAACCGCTCCGGGCATTTGCTGCATTTATCCTTCGTGATATCCGGACGTTGGCTGGTCATATTCATCCGCGACAGCCGGTTTTATAACCACTCATCACCGCGATAGGTCCAGGTGCAGGTGTTGGCCAGCATGATACGTCCCGGAAAAACAACGCGCCATCCGTTTCCGTCGGCGTGGACAGTACAAAAGGCACTCACCGCGCTCAGTTCGCTGCACTGCTCAATGCGCCAGCGACTGATCACCTCCTGCTCCGGATCGGCGTAACTGTTTCCGTTGACAGTTCACCGCATCCAGAAAACAGGCGTAAACCTTACGCGGACCACCGTTCCCGCCGACCAGACTCTGCATATCTTCCGCCATCCGGTGACCATACCGTACGGGTTGAAACCGTCAGCGTGGGGCTTGTGCTGGTGCCTTGCCATTCAGTTCAAACCGCTCCCTGAGATGGACCGGCTGATACTGTAACCTGCCGAGGTGACCGGCTCACCTTTTCGTTCTGCTGCATTACAGAAAAATAACGTTCTCCACCGACCTCTGTCAGGTCGTTTCCCAGAAGAGCACCACGCTGGCCGACTGCTCCGCACGGGTGCATTCATTCGGTTTCCTGCCGGATATCCTGCATCAGTTCACCACCTGTTCAAACTCTGCGCTGAACTCAACACGCAGCATACTGACCCGCGACGACCATTTGCGCAGGTCACCTTTATCTGCCGCCACTCATAAAGGCGGCGTCCACGAAAGGATTTCCAGCCCCGTGCTCTTCCAGAAACGACTTGGTACCGTAGCCTCCCTCCACGGGGGGACAGAAAGCGCGTCACACTGTACGTTTTCAGGTTGGCGACCTCAGCCCGGCAGGCGCTCGCTGAAATAGCCATCACCAAAGCGCACCTTTCTTACAGAAGGGACCGAAGCCACATCCATACCGGGTTTCACTTTCAGCGGAAGGTCTTCATCGTCCACCTCCGGAGAACAGGCCACCATCACGCATCTGTGTCTGAATTTCATCACGGGCGCCCTTGCGAATCATGTCATACACCGCCTTCAGAGCAGCCGGACCTATCTGCCCGTTCATTGCCGTCGTTGTTAATCACCATGGATTTATTCTGCTCAAACGTCCCGGACGCCTGCGACGGCTGTCCATGCTGCCCGGTGTACCGACACCACC");

    const std::string s2("TTAATTGCCGTTCGGCTGGTGGGTGTTTAACCGGTTTCGCATTTATCGTGAAACGCTCGCGTTTTTCGTGCGCCGCTTCAATATCCGCAGTCGAACACCGTTTGTAAATCGGCGTGGTGGGGCCGTCGGTCAGGCGACGATGATGATGCGGAAGTTACGGATTTTTTCAAGAGCGAATAACCGAATCCCATCTCAGCAGAAGACTGCTGGAAAAAATTCGAGCTGACGGAGGATAACTGGCTGAAGGTTTTCGAAGAATTGGAAGGATGCCAGTGATAGGTGAATACCATGTGGGCTGTCAAAATTGAGCAGATTGAACGTAACTAATAAGCGAATGGCCTCAGCATGGAGGACGTACGGGAAGAACAAACTGAAGCGGATTTCGGTTGCGCCAAAGTCGTATCGCATTTATTGACCCGGCAGAGGAATGAAGCGCCGATGTTTGTGGCGCAGGGCAACCAGATATTCACATGAACGACGTGTGCAAATACGCTGACGGCCCCACCATTACCAGCGGCAGCGTCCGCCAGCCTTCCTGACACCGGACTGAAGCTGACCGCTAAAAATGCTGGATATCAGTGGCAGATGTGAATGCGAACTCGGACACTCAGTAATGTGACGATAACTGTACGATAAACGCGTTACGCTGAGGGCGGAAAAAATCGTGGGGACATTTAGCAAAGGCAGCGAGCGCAGCTCCGCGCGCCAGCGGTAGCTGGTGGACTTGGCCGTTAGGTGCCGACCTGTCACCGGCCGATGACCATCCTTTTGATCGCCAGATAGTGGTGCTGTGACGTTTCGCGGAAATGCAGCGTACTGTCAGCGGCAGGACAACGGCTATTCGATGCTGGTATCTGAAAATTACTGATGAACGGTGCGGTGATTTATGATGGCGCGGCGAACGAGGCGGTACAGGTGTTCTCCGGCGTATTGGTGACATGCCAGCGGGTCGAGGAAACGTGATCCTGACGTTCACGCTTACGTCCACACGGCATTCGGCAGACTCCGCCGTATACGTTTGCCAGCGCGATATTACGGAGTTGGTGATTAAGCAGAGCGCTGGGCATCGCTGTGGTCTGGAAGATGTTACAGAGGTTCGTCCAAAGGGAACGGGCGTTATTATATAAAACATTGAGAGGTGGAGCGATGCGCGAAATGTGTGCCGCGTTTTGTCTTTGCCGCACTTGCGGTGACAGTCCTCCGGCCCGATGCGGAAGGTGGACATGTGCACGTTTACGGTGGGCTGGCCTGGTGGAAACCAGGTACATTACCGTCGTTGTCAGGCAGGGATACCGGGGTGTACGGTGTCTGAAAAGGATTGAAGTACCGTTATGAGCTGAAGGCGGACAGTGTGATGGCTTCCCTGAGGTTCCGCCGCGTGAAAAATTGGCA");

    BottomOverlapSketch b1(s1, 12, 1536, false);
    BottomOverlapSketch b2(s2, 12, 1536, false);

    auto overlapInfo = b1.getOverlapInfo(b2, 0.2);

    BOOST_CHECK_EQUAL(overlapInfo.score, 0.7790778080544442);
    BOOST_CHECK_EQUAL(overlapInfo.a1, 15);
    BOOST_CHECK_EQUAL(overlapInfo.a2, 74);
    BOOST_CHECK_EQUAL(overlapInfo.b1, 12);
    BOOST_CHECK_EQUAL(overlapInfo.b2, 69);
}

BOOST_AUTO_TEST_SUITE_END()
