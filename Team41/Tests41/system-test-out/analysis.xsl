<?xml-stylesheet type="text/xsl" href="analysis.xsl"?>
<test_results>
<info>
<name>autotester</name><parsing_time_taken>344.657000</parsing_time_taken>
</info>
<queries>
<query>
<id Affects="1" ReturnBoolean="1" UnnamedVar="2" SuchThat="1" CondNum="5" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[ Select BOOLEAN such that Affects(_, _)]]></querystr>
<stuans>TRUE</stuans>
<correct>TRUE</correct>
<time_taken>20.750000</time_taken>
<passed/>
</query>
<query>
<id ReturnTuple="1" Affects="1" UnnamedVar="1" SuchThat="1" CondNum="4" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[stmt s; Select s such that Affects(s, _)]]></querystr>
<stuans>62,49,80,9,38,63,6,100,42,87,101,5,19</stuans>
<correct></correct>
<time_taken>19.253000</time_taken>
<failed>
<missing></missing>
<additional>100,101,19,38,42,49,5,6,62,63,80,87,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>13</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" UnnamedVar="1" SuchThat="1" CondNum="4" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[stmt s; Select s such that Affects(_, s)]]></querystr>
<stuans>63,50,81,9,38,49,62,20,19,88,87,101,43,100,6</stuans>
<correct></correct>
<time_taken>19.923000</time_taken>
<failed>
<missing></missing>
<additional>100,101,19,20,38,43,49,50,6,62,63,81,87,88,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>15</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[stmt s; Select s such that Affects(s, s)]]></querystr>
<stuans>87,38,6,9,49,19</stuans>
<correct></correct>
<time_taken>19.526000</time_taken>
<failed>
<missing></missing>
<additional>19,38,49,6,87,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>6</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[stmt s1, s2; Select s such that Affects(s1, s2)]]></querystr>
<stuans>SemanticError</stuans>
<correct></correct>
<time_taken>1.328000</time_taken>
<failed>
<missing></missing>
<additional>SemanticError</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>1</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" UnnamedVar="1" SuchThat="1" CondNum="4" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[assign a; Select a such that Affects(a, _)]]></querystr>
<stuans>62,49,80,9,38,63,6,100,42,87,101,5,19</stuans>
<correct></correct>
<time_taken>20.296000</time_taken>
<failed>
<missing></missing>
<additional>100,101,19,38,42,49,5,6,62,63,80,87,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>13</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" UnnamedVar="1" SuchThat="1" CondNum="4" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[assign a; Select a such that Affects(_, a)]]></querystr>
<stuans>63,50,81,9,38,49,62,20,19,88,87,101,43,100,6</stuans>
<correct></correct>
<time_taken>19.250000</time_taken>
<failed>
<missing></missing>
<additional>100,101,19,20,38,43,49,50,6,62,63,81,87,88,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>15</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[assign a; Select a such that Affects(a, a)]]></querystr>
<stuans>87,38,6,9,49,19</stuans>
<correct></correct>
<time_taken>19.358000</time_taken>
<failed>
<missing></missing>
<additional>19,38,49,6,87,9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>6</additional>
</summary>
</failed>
</query>
<query>
<id ReturnTuple="1" Affects="1" SuchThat="1" CondNum="3" RelNum="1" comment="valid affects wildcard">1</id><querystr><![CDATA[assign a1, a2; Select <a1, a2> such that Affects(a1, a2)]]></querystr>
<stuans>62 63,80 81,9 9,49 49,49 50,87 87,42 43,5 6,63 62,38 38,87 88,100 101,101 100,6 6,9 20,19 19</stuans>
<correct></correct>
<time_taken>19.419000</time_taken>
<failed>
<missing></missing>
<additional>100 101,101 100,19 19,38 38,42 43,49 49,49 50,5 6,6 6,62 63,63 62,80 81,87 87,87 88,9 20,9 9</additional>
<summary>
<expected>0</expected>
<matched>0</matched>
<missing>0</missing>
<additional>16</additional>
</summary>
</failed>
</query>
</queries>
</test_results>