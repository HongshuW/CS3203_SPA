<?xml-stylesheet type="text/xsl" href="analysis.xsl"?>
<test_results>
<info>
<name>autotester</name><parsing_time_taken>6.952000</parsing_time_taken>
</info>
<queries>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects">1</id><querystr><![CDATA[assign a1; Select a1 such that Affects(a1, a1)]]></querystr>
<stuans>5,14,11,9,10,8,4,22,15</stuans>
<correct>10,11,12,14,15,22,4,5,8,9</correct>
<time_taken>57.387000</time_taken>
<failed>
<missing>12</missing>
<additional></additional>
<summary>
<expected>10</expected>
<matched>9</matched>
<missing>1</missing>
<additional>0</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects">1</id><querystr><![CDATA[assign a1, a2; Select <a1, a2> such that Affects(a1, a2)]]></querystr>
<stuans>10 18,10 15,8 9,5 15,10 16,14 15,8 15,4 5,9 18,11 15,9 15,5 18,15 16,14 16,22 22,4 7,15 18</stuans>
<correct>10,11,12,14,15,22,4,5,8,9</correct>
<time_taken>1.871000</time_taken>
<failed>
<missing>10,11,12,14,15,22,4,5,8,9</missing>
<additional>10 15,10 16,10 18,11 15,14 15,14 16,15 16,15 18,22 22,4 5,4 7,5 15,5 18,8 15,8 9,9 15,9 18</additional>
<summary>
<expected>10</expected>
<matched>0</matched>
<missing>10</missing>
<additional>17</additional>
</summary>
</failed>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects same statement">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(8, 8)]]></querystr>
<stuans>FALSE</stuans>
<correct>TRUE</correct>
<time_taken>1.490000</time_taken>
<failed>
<missing>TRUE</missing>
<additional>FALSE</additional>
<summary>
<expected>1</expected>
<matched>0</matched>
<missing>1</missing>
<additional>1</additional>
</summary>
</failed>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects same statement">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(15, 15)]]></querystr>
<stuans>FALSE</stuans>
<correct>TRUE</correct>
<time_taken>1.538000</time_taken>
<failed>
<missing>TRUE</missing>
<additional>FALSE</additional>
<summary>
<expected>1</expected>
<matched>0</matched>
<missing>1</missing>
<additional>1</additional>
</summary>
</failed>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects same statement">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(22, 22)]]></querystr>
<stuans>TRUE</stuans>
<correct>TRUE</correct>
<time_taken>2.116000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(4, 5)]]></querystr>
<stuans>TRUE</stuans>
<correct>True</correct>
<time_taken>2.083000</time_taken>
<failed>
<missing>True</missing>
<additional>TRUE</additional>
<summary>
<expected>1</expected>
<matched>0</matched>
<missing>1</missing>
<additional>1</additional>
</summary>
</failed>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(10, 16)]]></querystr>
<stuans>TRUE</stuans>
<correct>FALSE</correct>
<time_taken>1.582000</time_taken>
<failed>
<missing>FALSE</missing>
<additional>TRUE</additional>
<summary>
<expected>1</expected>
<matched>0</matched>
<missing>1</missing>
<additional>1</additional>
</summary>
</failed>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects in if">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(4, 7)]]></querystr>
<stuans>TRUE</stuans>
<correct>TRUE</correct>
<time_taken>1.335000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects separate if branches">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(7, 10)]]></querystr>
<stuans>FALSE</stuans>
<correct>FALSE</correct>
<time_taken>1.429000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects modified in procedure">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(12, 15)]]></querystr>
<stuans>FALSE</stuans>
<correct>FALSE</correct>
<time_taken>1.287000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects not modified in procedure">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(11, 15)]]></querystr>
<stuans>TRUE</stuans>
<correct>TRUE</correct>
<time_taken>1.242000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects not modified in procedure">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(10, 15)]]></querystr>
<stuans>TRUE</stuans>
<correct>TRUE</correct>
<time_taken>1.376000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects in separate procedures">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(13, 15)]]></querystr>
<stuans>FALSE</stuans>
<correct>FALSE</correct>
<time_taken>1.557000</time_taken>
<passed/>
</query>
<query>
<id Affects="1" ReturnBoolean="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects in separate procedures">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(19, 15)]]></querystr>
<stuans>FALSE</stuans>
<correct>FALSE</correct>
<time_taken>1.254000</time_taken>
<passed/>
</query>
</queries>
</test_results>
