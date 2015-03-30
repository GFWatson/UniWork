using UnityEngine;
using System.Collections;

public class IPspawn : MonoBehaviour {



	// Use this for initialization
	void Start () {

		string[] ipAddresses = new string[100];
		string address = "";
		for (int i = 0; i < 100; i++) {
			for (int x = 0; x < 4; x++) {
				int three = Random.Range(0, 1000);
				string threeGroup = three.ToString();
				if (three < 100 && three >= 10) {
					threeGroup = threeGroup + "0";
				}
				else if (three < 10) {
					threeGroup = threeGroup + "00";
				}
				if (x < 3) {
					address = address + threeGroup + ".";
				}
				else {
					address = address + threeGroup;;
				}
			}
			ipAddresses[i] = address;
			Debug.Log(address);
			address = "";
		}


	}
	
	// Update is called once per frame
	void Update () {


	}
}
