using UnityEngine;
using System.Collections;

public class Spawn : MonoBehaviour {

	public GameObject box;

	// Use this for initialization
	void Start () {
		InvokeRepeating ("spawn", 2.0f, 2.0f);
	
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void spawn() {

		Instantiate(box, new Vector3(-28, 35, 49), Quaternion.identity);
		Instantiate(box, new Vector3(-9, 35, 49), Quaternion.identity);
		Instantiate(box, new Vector3(10, 35, 49), Quaternion.identity);
	}
}
