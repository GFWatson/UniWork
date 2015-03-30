using UnityEngine;
using System.Collections;

public class Translate : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		float moveSpeed = 5f;
		transform.Translate(Vector3.down * moveSpeed * Time.deltaTime);
		if (transform.position.y <= -30) {
			Destroy (gameObject);
		}
	}
}
